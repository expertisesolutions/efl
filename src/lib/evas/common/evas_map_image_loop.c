#ifdef SMOOTH
{
# ifdef SCALE_USING_MMX
#  ifdef COLMUL
#   ifdef COLSAME
   MOV_P2R(c1, mm7, mm0); // col
#   endif   
#  endif   
# endif
# ifdef SCALE_USING_NEON
   FPU_NEON;
   VMOV_I2R_NEON(q2, #255);
#  ifdef COLMUL
#   ifndef COLBLACK
   // this part can be done here as c1 and c2 are constants in the cycle
   FPU_NEON;
   VMOV_M2R_NEON(d18, c1);
   VEOR_NEON(q8);
#    ifndef COLSAME
   VMOV_M2R_NEON(d19, c2);
#    endif
   VZIP_NEON(q9, q8);
#    ifndef COLSAME
   VMOV_R2R_NEON(d19, d16);
#    endif
   // here we have c1 and c2 spread through q9 register
#   endif
#  endif
# endif
     while (ww > 0)
     {
# ifdef COLBLACK
        *d = 0xff000000; // col
# else        
        FPc u1, v1, u2, v2;
        FPc rv, ru;
        DATA32 val1, val2, val3, val4;
        
        u1 = u;
        if (u1 < 0) u1 = 0;
        else if (u1 >= swp) u1 = swp - 1;
        
        v1 = v;
        if (v1 < 0) v1 = 0;
        else if (v1 >= shp) v1 = shp - 1;
        
        u2 = u1 + FPFPI1;
        if (u2 >= swp) u2 = swp - 1;
        
        v2 = v1 + FPFPI1;
        if (v2 >= shp) v2 = shp - 1;
        
        ru = (u >> (FP + FPI - 8)) & 0xff;
        rv = (v >> (FP + FPI - 8)) & 0xff;
        
        s = sp + ((v1 >> (FP + FPI)) * sw) + 
          (u1 >> (FP + FPI));
        val1 = *s;
        s = sp + ((v1 >> (FP + FPI)) * sw) + 
          (u2 >> (FP + FPI));
        val2 = *s;
        
        s = sp + ((v2 >> (FP + FPI)) * sw) + 
          (u1 >> (FP + FPI));
        val3 = *s;
        s = sp + ((v2 >> (FP + FPI)) * sw) + 
          (u2 >> (FP + FPI));
        val4 = *s;
#  ifdef SCALE_USING_MMX
        MOV_A2R(rv, mm4);
        MOV_A2R(ru, mm6);
        MOV_P2R(val1, mm1, mm0);
        if (val1 | val2)
          {
             MOV_P2R(val2, mm2, mm0);
             INTERP_256_R2R(mm6, mm2, mm1, mm5);
          }
        MOV_P2R(val3, mm2, mm0);
        if (val3 | val4)
          {
             MOV_P2R(val4, mm3, mm0);
             INTERP_256_R2R(mm6, mm3, mm2, mm5);
          }
        INTERP_256_R2R(mm4, mm2, mm1, mm5);
#   ifdef COLMUL
#    ifdef COLSAME
//        MOV_P2R(c1, mm7, mm0); // col
        MUL4_SYM_R2R(mm7, mm1, mm5); // col
#    else        
        cc = cv >> 16; // col
        cv += cd; // col
        MOV_A2R(cc, mm2); // col
        MOV_P2R(c1, mm3, mm0); // col
        MOV_P2R(c2, mm4, mm0); // col
        INTERP_256_R2R(mm2, mm4, mm3, mm5); // col
        MUL4_SYM_R2R(mm3, mm1, mm5); // col
#    endif        
#   endif                            
        MOV_R2P(mm1, *d, mm0);
#  elif defined SCALE_USING_NEON
        // not sure if we need this condition, but it doesn't affect the result
        if (val1 | val2 | val3 | val4)
          {
            FPU_NEON;
#   ifdef COLMUL
            // initialize alpha for interpolation of c1 and c2
            VDUP_NEON(d15, cv >> 16);
            // copy c1 and c2 as algorithm will overwrite it
            VMOV_R2R_NEON(q6, q9);
            cv += cd; // col
#   endif
            VMOV_M2R_NEON(d8, val1);
            VEOR_NEON(q0);
            VMOV_M2R_NEON(d9, val3);
            VMOV_M2R_NEON(d10, val2);
            VEOR_NEON(q1);
            VMOV_M2R_NEON(d11, val4);
            VDUP_NEON(q3, ru);
            VDUP_NEON(d14, rv);
            VZIP_NEON(q4, q0);
            VZIP_NEON(q5, q1);
            VMOV_R2R_NEON(d9, d0);
            VMOV_R2R_NEON(d11, d2);
            // by this point we have all required data in right registers
            INTERP_256_NEON(q3, q5, q4, q2); // interpolate val1,val2 and val3,val4
#   ifdef COLMUL
#    ifdef COLSAME
            INTERP_256_NEON(d14, d9, d8, d4);
#    else
            VSWP_NEON(d9, d12); // move result of val3,val4 interpolation (and c1 if COLMUL is defined) for next step
            INTERP_256_NEON(q7, q6, q4, q2); // second stage of interpolation, also here c1 and c2 are interpolated
#    endif
#   else
            INTERP_256_NEON(d14, d9, d8, d4);
#   endif
#   ifdef COLMUL
#    ifdef COLSAME
            MUL4_SYM_NEON(d8, d12, d4);
#    else
            MUL4_SYM_NEON(d8, d9, d4); // do required multiplication
#    endif
#   endif
            VMOV_R2M_NEON(q4, d8, d); // save result to d
          }
        else
          *d = val1;
#  else
        val1 = INTERP_256(ru, val2, val1);
        val3 = INTERP_256(ru, val4, val3);
        val1 = INTERP_256(rv, val3, val1); // col
#   ifdef COLMUL                            
#   ifdef COLSAME
        *d = MUL4_SYM(c1, val1);
#   else        
        val2 = INTERP_256((cv >> 16), c2, c1); // col
        *d   = MUL4_SYM(val2, val1); // col
        cv += cd; // col
#   endif
#   else                            
        *d   = val1;
#   endif
#  endif
        u += ud;
        v += vd;
# endif        
        d++;
        ww--;
     }
}
#else
{
# ifdef SCALE_USING_NEON
#  ifdef COLMUL
#   ifndef COLBLACK
#    ifdef COLSAME
   FPU_NEON;
   VMOV_I2R_NEON(q2, #255);
   VMOV_M2R_NEON(d10, c1);
   VEOR_NEON(d0);
   VZIP_NEON(d10, d0);
#    else
   // c1 and c2 are constants inside the cycle
   FPU_NEON;
   VMOV_I2R_NEON(q2, #255);
   VMOV_M2R_NEON(d10, c1);
   VEOR_NEON(q0);
   VMOV_M2R_NEON(d11, c2);
   VZIP_NEON(q5, q0);
   VMOV_R2R_NEON(d11, d0);
#    endif
#   endif
#  endif
# endif
   while (ww > 0)
     {
# ifdef COLMUL
#  ifndef COLBLACK
        DATA32 val1;
#   ifdef COLSAME
#   else        
        DATA32 cval; // col
#   endif        
#  endif
# endif
# ifdef COLBLACK
        *d = 0xff000000; // col
# else
        s = sp + ((v >> (FP + FPI)) * sw) + 
          (u >> (FP + FPI));
#  ifdef COLMUL
        val1 = *s; // col
#   ifdef COLSAME
#    ifdef SCALE_USING_NEON
        VMOV_M2R_NEON(d1, val1);
        VEOR_NEON(d0);
        VZIP_NEON(d1, d0);
        VMOV_R2R_NEON(d0, d10);
        MUL4_SYM_NEON(d0, d1, d4)
        VMOV_R2M_NEON(q0, d0, d);
#    else
        *d = MUL4_SYM(c1, val1);
#    endif
#   else
#    ifdef SCALE_USING_NEON
        FPU_NEON;
        VMOV_M2R_NEON(d12, val1);
        VMOV_R2R_NEON(q4, q5);
        VEOR_NEON(q1);
        VDUP_NEON(d15, cv >> 16);
        VZIP_NEON(q6, q1);
        INTERP_256_NEON(d15, d9, d8, d4); // interpolate c1 and c2
        MUL4_SYM_NEON(d8, d12, d4); // multiply
        VMOV_R2M_NEON(q4, d8, d); // save result
#    else
        cval = INTERP_256((cv >> 16), c2, c1); // col
        *d = MUL4_SYM(cval, val1);
        cv += cd; // col              
#    endif
#   endif        
#  else
        *d = *s;
#  endif
        u += ud;
        v += vd;
# endif
        d++;
        ww--;
     }
}
#endif
