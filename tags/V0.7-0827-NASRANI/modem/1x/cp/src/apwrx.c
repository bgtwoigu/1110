/****************************************************************************************
*
*   Title:    APWRXARM.C
*
*   Author:   R. F. Quick, Jr.
*   Created:  April 21, 1998
*   Modified: January 13, 1999
*
*   Language: Microsoft Visual C++ 5.0; ARM ?
*
*   Description:
*
*     Provides the n-bit a**x mod p function required for Diffie-Hellman
*     key exchange.
*
*     Note: this procedure returns 1 for 0**0.
*     Note: this procedure requires that p be an odd number (lsb must be 1)
*
*     This version implements Montgomery reduction and 4-bit
*     sliding-window exponent.  Also has special squaring routine.
*
*     This version uses 32-bit arithmetic, C only.
*
*   Template:
*
*     int apwrxN(int n, unsigned WORD *a, unsigned WORD *x, unsigned WORD *ax,
*       unsigned WORD *p);
*
*   Calling parameters:
*
*     n Number of bits in a, x and p.
*
*     a Input: N-bit value of a.  Stored as unsigned WORD array, with the 
*       least significant word in a[0].  Not changed during execution.
*       
*     x Input: N-bit value of x.  Stored as unsigned WORD array, with the 
*       least significant word in a[0].  Not changed during execution.
*       
*     ax  Output: N-bit value of a**x mod p.  Stored as unsigned WORD array, 
*       with the least significant word in a[0].
*
*     p Input: N-bit value of p.  Stored as unsigned WORD array, with the 
*       least significant word in a[0].  Not changed during execution.
*       
*   Return value:
*   
*     0 if no error, -1 if error.  The  error return occurs if p is all zero
*     or is an even number.
*         
*   (c) 1998, 1999 QUALCOMM Incorporated
*

Copyright (c) 1998-2005 by QUALCOMM, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$PVCSPath: L:/src/asw/MSM5100/CP_REL_A/vcs/apwrx.c_v   1.0.2.0   30 Nov 2001 16:43:58   fchan  $
$Header: //source/qcom/qct/modem/1x/cp/rel/1h08/src/apwrx.c#1 $ $DateTime: 2009/05/29 03:54:56 $ $Author: prabhuk $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/26/06   an      Fixed Lint Errors
05/16/05   fc      Merged the following from MSMSHARED:
05/12/05   az       Fixed lint errors
11/20/02   va      Fixed compiler warning.
09/30/99   ks      Removed compiler warnings by added casts.
01/25/99   ck      Added code to acknowledge the STOP and OFFLINE signals if
                   they are recd in the middle of exponentiation.
01/20/99   ck      Checked in the Module with changes to generate a random
                   number, report to dog and to abort the processing

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "target.h"

#if(defined (FEATURE_DH) && defined(FEATURE_DH_EXP))

#include "memory.h"
#include "apwrx.h"
#include "dog.h"
#include "msg.h"
#include "dh.h"
#include "task.h"
#include "mc.h"

/* WindowSize must be a power of 2 from 1 to 3
   (i.e. WindowSize can be 2, 4 or 8 bits) */

/* Suppress the following lint warnings 
737 : Loss of sign
713, 734 : Loss of precision
669 : Possible data overrun
670 : Possible access beyond array
574,573 : Signed-unSigned mix in relational/divide
514 : Unusual use of boolean
504 : Unusual Shift operation
668 : Possible use of null pointer
*/
/*lint -e737 -e713  -e734 -e669 -e670 -e574 -e573 -e514 -e504 -e668 -e818*/
#define WindowSize 4
#define WindowStorage ((1 << WindowSize) - 1)
#define WindowMask ((1 << WindowSize) - 1)

static 
void WORD_mul(unsigned WORD xy[2],  /* product */
         unsigned WORD x[2],  /* first multiplier */      
         unsigned WORD y)   /* second multiplier */
{
  unsigned WORD t,u;

  /* create x0y0 and x1y1 terms and store */
  xy[0] = (x[0] & LOW)*(y & LOW);
  xy[1] = (x[0] >> (BITCT/2))*(y >> (BITCT/2));

  /* create cross terms in t and u */
  t = (x[0] & LOW)*(y >> (BITCT/2));
  u = (x[0] >> (BITCT/2))*(y & LOW);

  /* add cross terms with carry to xy[1] */
  t += u;
  if (t < u)
    xy[1] += 1 << (BITCT/2);

  /* add ls part of cross term to xy[0] with carry to xy[1] */
  u = xy[0] + (t << (BITCT/2));
  xy[1] += (u < xy[0]); /*lint !e514*/
  xy[0] = u;

  /* add ms part of cross term to xy[1] */
  xy[1] += t >> (BITCT/2);
}

/* multiply multiple-precision numbers
   the extra word in xy ensures that the addition loop terminates
   with no carry, eliminating the need for checking the index */

static
void mulN(int N,        /* number of unsigned WORDs in x and y */
      unsigned WORD *x,   /* first multiplier */
      unsigned WORD *y,   /* second multiplier */
      unsigned WORD *xy)    /* product (2N+1 in size) */
{
  int i,j,k;
  unsigned WORD t[2], tx[2];

  memset(xy, 0, 2*N*sizeof(unsigned WORD)); /*lint !e737*/

  tx[1] = 0;

  for (i = 0; i < N; i++)
  {
    for (j = 0; j < N; j++)
    {
      tx[0] = x[i];
      WORD_mul(t,tx,y[j]);

      for (k = i+j; k < 2*N; k++)
      {
        xy[k] += t[0];
        t[0] = t[1] + (xy[k] < t[0]); /*lint !e514*/
        if (t[0] == 0)
          break;
        t[1] = 0;
      }
    }
  }
}

/* square a multiple-precision number */

static
void squareN(int N,
       unsigned WORD *x,
       unsigned WORD *x2)
{
  int i,j,k;
  unsigned WORD c, d, t[2], tx[2];

  tx[1] = 0;

  for (i = 0; i < N; i++)
  {
    /* put in diagonal terms (sets x2, hence no need to initialize) */

    tx[0] = x[i];
    WORD_mul(&x2[2*i],tx,x[i]);
  }

  for (i = 0; i < N-1; i++)
  {
    /* add in twice the off-diagonal terms */

    for (j = i+1; j < N; j++)
    {
      tx[0] = x[i];
      WORD_mul(t,tx,x[j]);

      /* double t */

      d = t[0] << 1;
      c = (d < t[0]);
      t[0] = d;

      d = (t[1] << 1) + c;
      c = (d < t[1]);
      t[1] = d;
      d = c; /* save carry off end */

      for (k = i+j; k < 2*N; k++)
      {
        /* add lsw with carry to c */

        x2[k] += t[0];
        c = (x2[k] < t[0]);

        /* add c to msw with carry to d */

        t[1] += c;
        d += (t[1] < c); /*lint !e514*/

        if ((t[1]|d) == 0)
          break;

        t[0] = t[1];
        t[1] = d;
        d = 0;
      }
    }
  }
}

/* find the integer quotient from dividing an unsigned WORD into a double unsigned WORD */

static
unsigned WORD divd1(unsigned WORD d[2], unsigned WORD v)
{
  int i, c;
  unsigned WORD q;

  if (v == 0) // actual first word should have been 10...0, just divide by 2**BITCT
  {
    if (d[1])
      return d[1];
    else  // special case where divisor is 0xf...f and d[] is also 0xf...f
      return 1;
  }

  if (d[1] == 0) // ms word is zero
    return d[0]/v;

  q = 0;
  c = 0;
  for (i = 0; i < BITCT; i++) /*lint !e574 !e737*/
  {
    q <<= 1;

    if (d[1] & MSB)
      c = 1;
    d[1] <<= 1;
    if (d[0] & MSB)
      d[1]++;
    d[0] <<= 1;

    if (c || (v <= d[1]))
    {
      q++;
      d[1] -= v;
      c = 0;
    }
  }
  return q;
}

/* take xy % p.  p_mspos is the position of the most-significant nonzero word in p */

int modpN(int N,      /* number of unsigned WORDs in p */
      unsigned WORD *xy, /* product to be reduced (2N in size) */
      unsigned WORD *p, /* modulus */
      int xy_order    /* number of unsigned WORDs in xy */
     )
{
  int i,j,k,p_mspos;
  unsigned WORD q,t[2],tx[2],d;

  /* start with shifted p aligned with msw of xy, and step to the right,
     subtracting multiples of shifted p until xy is less than shifted p */

  /* find ms word of p */

  for (p_mspos = N-1; p_mspos >= 0; p_mspos--)
    if (p[p_mspos] != 0)
      break;

  if (p_mspos < 0)
  /* p is zero; divide would hang */
  {
    return(-1);
  }

  /* set d to msw of divisor (doesn't change during this function) */
  
  d = p[p_mspos];
  tx[1] = 0;

  for (i = xy_order-1; i >= p_mspos; i--)
  {
    if (i < xy_order-1)
      t[1] = xy[i+1];
    else
      t[1] = 0;
    t[0] = xy[i];
    while (t[1] || (t[0] >= d))
    /* loop until xy is less than shifted p */
    {
      /* Get q = approximate quotient for this step.  Compute it using
         d+1 instead of d to allow for rounding error (thereby ensuring no
         overflow during the multiplication of p by q).
         
         q can be anywhere from half the actual quotient to
         the actual quotient, depending on how much larger d+1 is compared
         to d. (With left-justified p, the range is much
         tighter, speeding up the division.) */

      if (t[1] || (t[0] > d))
        q = divd1(t, (unsigned WORD)(d+1));
      else if (t[0] < d)
        q = 0;
      else
      /* see if xy is really >= shifted p */
      {
        q = 1;
        for (j = i-1; j >= i - p_mspos; j--)
        {
          if (xy[j] > p[p_mspos+j-i])
          /* it's bigger, go do the subtraction */
            break;
          else if (xy[j] < p[p_mspos+j-i])
          /* it's smaller, stop */
          {
            q = 0;
            break;
          }
          /* continue only if the words are equal */
        }
      }

      if (q == 0)
      /* xy is less than shifted p, break out of while loop */
        break;

      /* subtract p*q from xy */

      for (j = 0; j <= p_mspos; j++)
      {
        tx[0] = p[j];
        WORD_mul(t,tx,q);

        /* subtract from xy, propagating borrow to msw */

        for (k = (j-p_mspos+i); k < 2*N; k++)  /*lint !e834 */
        {
          if (t[0] > xy[k])
            /* there's a borrow */
            t[1]++;
          xy[k] -= t[0];
          if (t[1] == 0)
            break;
          t[0] = t[1];
          t[1] = 0;
        }
      }
      /* set t again for loop test */
      if (i < xy_order-1)
        t[1] = xy[i+1];
      else
        t[1] = 0;
      t[0] = xy[i];
    } /* while >= d */
  } /* for i */

  return(0);
}

/* set up the Montgomery information.  */

void InitMont(unsigned WORD *m,   /* storage for -1/p */
        unsigned WORD *p)   /* modulus */
{
  unsigned WORD x, t, u;

  /* determine m.  m is such that m*p[0] modulo 2**BITCT is -1.  Note this
     routine only works if p is odd. */

  x = p[0];
  t = 1;
  u = 2;
  while(u) 
  {
    if ((x*t) & u)
      t += u;
    u <<= 1;
  }
  
  *m = (unsigned WORD)(-(WORD)t);  /* this is to avoid compiler complaints */
}

/* Montgomery reduction */

static
void MontRedN(int N,          /* number of unsigned WORDs in p */
        unsigned WORD *xxr,   /* 2N+1 entries */
        unsigned WORD m,      /* -1/p */
        unsigned WORD *p)     /* modulus */
{
  int i,j,k;
  unsigned WORD c, t[2], tx[2];
  unsigned WORD v, w;

  tx[1] = xxr[2*N] = 0;

  for (i = 0; i < N; i++)
  {
    /* create Montgomery term v = xxr[i]*m */

    v = xxr[i]*m;

    /* add v*p to xxr */

    for (j = 0; j < N; j++)
    {
      tx[0] = p[j];
      WORD_mul(t,tx,v);

      for (k = i+j; k <= 2*N; k++)
      {
        xxr[k] += t[0];
        t[0] = t[1] + (xxr[k] < t[0]);  /*lint !e514*/
        t[1] = 0;

        if (t[0] == 0)
          break;
      }
    }
  }

  t[0] = xxr[2*N];  /* xxr[2*N] holds any carries off end */

  /* if no carry, compare result to p, subtract if greater than or equal */

  if (t[0] == 0)
  {
    t[0] = 1;
    for (i = N-1; i >= 0; i--)
    {
      if (p[i] != xxr[i+N])
      {
        if (p[i] > xxr[i+N])
          t[0] = 0;
        break;
      }
    } /* falls off end if equal, with t[0] set to 1 */
  }
  else
    t[0] = 2;

  /* if there was a carry off the end or else if p < xxr, subtract p */

  if (t[0])
  {
    c = 0;
    for (k = 0; k < N; k++)
    {
      w = xxr[k+N];
      v = w - c;
      c = (v > w);
      w = v - p[k];
      c += (w > v); /*lint !e514*/
      xxr[k+N] = w;
    }
  }
}

/* Montgomery a**x mod p */

/*  temporary storage:

  pb:   copy of modulus, dimension Nmax
  axb:  intermediate result, dimension Nmax
  at:   temp used for multiplications, dimension 2*Nmax+1
  ap:   window elements, dimension WindowStorage*Nmax

*/

static unsigned WORD apwrxNtemp[1+((4+WindowStorage)*Nmax)];

int apwrxN(int n,       /* number of bits in p */
       unsigned WORD *a,  /* generator */
       unsigned WORD *x,  /* exponent */
       unsigned WORD *ax, /* result */
       unsigned WORD *p   /* modulus */
      )
{
  int i,j, N;
  int xsize, xisone;
  unsigned WORD mask, m;
  unsigned WORD *axb, *pb, *at, *ap;
  word sigs;  /* Mask of signals returned by rex */


  /* if p is not an odd number, return error */

  if (!(p[0] & 1))
    return(-1);

  /* compute number of unsigned WORDs in n */

  N = (n+BITCT-1)/BITCT; /*lint !e737 !e713*/

  if (N > Nmax) /*lint !e737 !e574*/
    return(-1);

  /* set pointers */

  axb = apwrxNtemp;
  pb = axb+N;
  at = pb+N;
  ap = at + 2*N+1;

  /* copy the modulus into the buffer; this is to ensure that all arguments
     to the assembly routines have the same segment offset. */

  memcpy(pb, p, N*sizeof(unsigned WORD)); /*lint !e737*/

  /* note that by making R and R2 static one could avoid repeating the init step
     for the second exponentiation of Diffie-Hellman */

  InitMont(&m, pb);

  /* compute aR = a*(2**(BITCT*N)) mod p and save in ap[0] */

  memcpy(&at[N], a, N*sizeof(unsigned WORD)); /*lint !e737 !e669*/
  memset(at, 0, N*sizeof(unsigned WORD)); /*lint !e737*/
  if (modpN(N,at,pb,2*N))
    return(-1);
  memcpy(ap, at, N*sizeof(unsigned WORD)); /*lint !e737*/

  /* set axb to R mod p */

  memset(at, 0, N*sizeof(unsigned WORD)); /*lint !e737*/
  at[N] = 1;
  if (modpN(N,at,pb,N+1))
    return(-1);
  memcpy(axb, at, N*sizeof(unsigned WORD)); /*lint !e737*/

  xisone = 1;

  /* set xsize as bit shift to last nonzero word of x */

  /* find leftmost nonzero word */
  for (i = N-1; i >= 0; i--)
    if (x[i] != 0)
      break;

  if (i < 0)
  /* x is zero, the answer is 1 */
  {
    memset(&ax[1], 0, (N-1)*sizeof(unsigned WORD)); /*lint !e737*/
    ax[0] = 1;
    return(0);
  }

  /* set xsize to number of bits in nonzero words */

  xsize = (BITCT/WindowSize)*(i+1); /*lint !e737 !e713*/
  
  /* adjust to most significant nonzero bit of x[i] */
  
  mask = (unsigned WORD)WindowMask << (BITCT - WindowSize);
  while (mask != 0)
  {
    if (x[i] & mask)
      break;
    xsize--;
    mask >>= WindowSize;
  }

  /* initialize the x**n values */

  for (i = 0; i < WindowStorage/2; i++)
  {
    /* square the value at i (x**(i+1)) to produce the value at 2*i+1 */

    squareN(N, &ap[N*i], at);
    MontRedN(N, at, m, pb);
    memcpy(&ap[N*(2*i+1)], &at[N], N*sizeof(unsigned WORD)); /*lint !e737 !e669 !e670*/

    /* multiply the value at 0 by the value at 2*i+1 to produce
       the value at 2*i+2 */

    mulN(N, ap, &ap[N*(2*i+1)], at);
    MontRedN(N, at, m, pb);
    memcpy(&ap[N*(2*i+2)], &at[N], N*sizeof(unsigned WORD)); /*lint !e737 !e669 !e670*/
  }

  /* multiply by appropriate power of a for each nonzero bit of x */

  for (i = xsize-1; i >= 0; i--)
  {
    if (!xisone)
    {
      for (j = 0; j < WindowSize; j++)
      {
        squareN(N,axb,at);
        MontRedN(N,at,m,pb);
        memcpy(axb, &at[N], N*sizeof(unsigned WORD)); /*lint !e737 !e670*/
      }
    }

    j = (x[i/(BITCT/WindowSize)] >> WindowSize*(i%(BITCT/WindowSize))) & WindowMask; /*lint !e737 !e573 !e504*/
    
    if (j)
    /* multiply ax by a**n (depending on window), result in ax */
    {
      xisone = 0;

      /* multiply axb by ap[0]**j, mod p */
      
      mulN(N, &ap[N*(j-1)], axb, at);
      MontRedN(N, at, m, pb);
      memcpy(axb, &at[N], N*sizeof(unsigned WORD)); /*lint !e737 !e670*/
    }

    sigs = rex_get_sigs(&dh_tcb); /*lint !e734*/
    if ((sigs & DH_RPT_TIMER_SIG) != 0)
    {
       /* -----------------------------
       ** Kick watchdog and reset timer
       ** ----------------------------- */
       dog_report(DOG_DH_RPT);
       (void) rex_set_timer( &dh_rpt_timer, DOG_DH_RPT_TIME );
    }

    if ((sigs & DH_RAND_USED_SIG) != 0)
    {
       /* -----------------------------
       ** Generate random number
       ** ----------------------------- */
       MSG_MED(" RAND_USED_SIG received",0,0,0);
       (void) rex_clr_sigs( &dh_tcb, DH_RAND_USED_SIG );
       generate_rand();
    }

    if ((sigs & DH_ABORT_EXP_SIG) != 0)
    {
       /* -------------------------------
       ** Abort the processing and return
       ** ----------------------------- */
       MSG_MED(" DH_ABORT_EXP_SIG received",0,0,0);
       (void) rex_clr_sigs( &dh_tcb, DH_ABORT_EXP_SIG );
       return(0);
    }
    if ((sigs & TASK_STOP_SIG) != 0)
    {
       /* -------------------------------
       ** ACK the STOP signal and return
       ** ----------------------------- */
       MSG_MED(" TASK_STOP_SIG received",0,0,0);
       (void) rex_clr_sigs( &dh_tcb, TASK_STOP_SIG );
       (void) rex_set_sigs( &mc_tcb, MC_ACK_SIG);
       return(0);
    }
    if ((sigs & TASK_OFFLINE_SIG) != 0)
    {
       /* -------------------------------
       ** ACK the OFFLINE signal and return
       ** ----------------------------- */
       MSG_MED(" TASK_OFFLINE_SIG received",0,0,0);
       (void) rex_clr_sigs( &dh_tcb, TASK_OFFLINE_SIG );
       (void) rex_set_sigs( &mc_tcb, MC_ACK_SIG);
       return(0);
    }
  }

  /* last reduction removes R */

  memcpy(at, axb, N*sizeof(unsigned WORD)); /*lint !e737*/
  memset(&at[N], 0, N*sizeof(unsigned WORD)); /*lint !e737 !e669*/
  MontRedN(N, at, m, pb);

  /* store result in actual ax */

  memcpy(ax, &at[N], N*sizeof(unsigned WORD)); /*lint !e737 !e670*/

  return(0);
}
/*lint +e737 +e713  +e734 +e669 +e670 +e574 +e573 +e514 +e504 +e668 +e818*/
#endif /* FEATURE_DH and FEATURE_DH_EXP */

