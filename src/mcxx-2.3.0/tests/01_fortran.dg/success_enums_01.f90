! <testinfo>
! test_generator="config/mercurium-fortran run"
! compile_versions="default f2003"
! test_FFLAGS_default=""
! test_FFLAGS_f2003="-std=f2003"
! </testinfo>

PROGRAM MAIN
      ENUM, BIND(C)
          ENUMERATOR :: A, B, C
      END ENUM

      PRINT *, A, B, C
      IF (A /= 0) STOP 1
      IF (B /= 1) STOP 2
      IF (C /= 2) STOP 3
END PROGRAM MAIN
