dnl
dnl Copyright (C) 2009
dnl Barcelona Supercomputing Center - Centro Nacional de Supercomputacion
dnl Universitat Politecnica de Catalunya
dnl
dnl This program is free software; you can redistribute it and/or modify it
dnl under the terms of the GNU General Public License as published by the
dnl Free Software Foundation; either version 2 of the License, or (at your
dnl option) any later version.
dnl
dnl This program is distributed in the hope that it will be useful, but
dnl WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
dnl General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License along
dnl with this program; if not, write to the Free Software Foundation, Inc.,
dnl 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
dnl

dnl -----------------------------------------------------------------------
dnl Check for pin.
dnl -----------------------------------------------------------------------

include('ax_python.m4')

AC_DEFUN([AC_CHECK_DR], [
    ac_have_dr="no"
    enable_dr="no"
    AC_ARG_WITH([dr-path],
        AS_HELP_STRING([--with-dr-path], [Sets the path to DynamoRIO]),
        dr_path="$withval"
        enable_dr="yes",)
    
    if test "x${enable_dr}" = "xyes"; then
        AC_MSG_NOTICE([Using $dr_path for DynamoRIO])
        AX_CHECK_COMPILE_FLAG(-march=armv8-a, ax_cv_support_armv8=yes, [])
        AX_CHECK_COMPILE_FLAG(-march=armv7, ax_cv_support_armv7=yes, [])
        if test x"$ax_cv_support_armv8" = x"yes"; then
            AC_MSG_NOTICE([DynamoRIO configured for ARM_64.])
            dr_define="-DLINUX -DARM_64"
        elif test x"$ax_cv_support_armv7" = x"yes"; then
            AC_MSG_NOTICE([DynamoRIO configured for ARM_32.])
            dr_define="-DLINUX -DARM_32"
        else
            AC_MSG_NOTICE([DynamoRIO configured for X86_64.])
            dr_define="-DLINUX -DX86_64"
        fi
dnl        if test ${compile_pintools} = "yes"; then
            AC_SUBST(DR_CPPFLAGS,
                        ["$dr_define -I${dr_path}/include -I${dr_path}/ext/include"])
            AC_SUBST(DR_CFLAGS,
                        ["-fno-strict-aliasing -fno-stack-protector"])
            AC_SUBST(DR_LDFLAGS,
                        ["-Wl,--hash-style=sysv -Wl,-Bsymbolic \
                          -Wl,-rpath -Wl,${dr_path}/lib64 -Wl,-rpath -Wl,${dr_path}/lib64/debug -Wl,-rpath -Wl,${dr_path}/ext/lib64 \
                          -Wl,-rpath -Wl,${dr_path}/ext/lib64/debug -Wl,-rpath -Wl,${dr_path}/lib64/release -Wl,-rpath -Wl,${dr_path}/ext/lib64/release \
                          -L${dr_path}/lib64 -L${dr_path}/lib64/debug -L${dr_path}/ext/lib64 \
                          -L${dr_path}/ext/lib64/debug -L${dr_path}/lib64/release -L${dr_path}/ext/lib64/release"])
            AC_SUBST(DR_LIBRARIES,
                        ["-ldynamorio -ldrx -ldrutil -ldrmgr -ldrreg -ldrsyms -ldrwrap"])
dnl            AC_SUBST(XED2_PATH,
dnl            			["${pin_path}/extras/${xed2_dir}"])
           ac_have_dr="yes"
dnl        fi
    else
        AC_MSG_NOTICE([DynamoRIO tools have not been enabled])
    fi
    AM_CONDITIONAL([HAVE_DR], [test "x$ac_have_dr" = "xyes"])
])

