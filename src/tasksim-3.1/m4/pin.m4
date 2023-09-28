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

AC_DEFUN([AC_CHECK_PIN], [
    ac_have_pin="no"
    enable_pin="no"
    AC_ARG_WITH([pin-path],
        AS_HELP_STRING([--with-pin-path], [Sets the path to PIN]),
        pin_path="$withval"
        enable_pin="yes",)

    if test "x${enable_pin}" = "xyes"; then
        AC_MSG_NOTICE([Using $pin_path for PIN])
        compile_pintools="no"

        AC_CHECK_SIZEOF(void *)
        if test ${ac_cv_sizeof_void_p} -eq 4; then
            pin_define="-DTARGET_LINUX -DTARGET_IA32 -DHOST_IA32"
            pin_base_dir="$pin_path/ia32"
            xed2_dir="xed2-ia32"
            compile_pintools="yes"
        elif test ${ac_cv_sizeof_void_p} -eq 8; then
            pin_define="-DTARGET_LINUX -DTARGET_IA32E -DHOST_IA32E"
            pin_base_dir="$pin_path/intel64"
            xed2_dir="xed2-intel64"
            compile_pintools="yes"
        fi

        pin_define="-DBIGARRAY_MULTIPLIER=1 -DUSING_XED $pin_define"

        if test ${compile_pintools} = "yes"; then
            AC_SUBST(PIN_CPPFLAGS,
                        ["$pin_define \
                          -I${pin_path}/source/include/pin -I${pin_path}/source/include/pin/gen \
                          -I${pin_path}/extras/components/include \
                          -I${pin_path}/extras/${xed2_dir}/include"])
            AC_SUBST(PIN_CFLAGS,
                        ["-fno-strict-aliasing -fno-stack-protector"])
            AC_SUBST(PIN_LDFLAGS,
                        ["-Wl,--hash-style=sysv -Wl,-Bsymbolic -Wl,--version-script=${pin_path}/source/include/pin/pintool.ver \
                          -Wl,-rpath -Wl,${pin_base_dir}/lib -Wl,-rpath -Wl,${pin_base_dir}/lib-ext \
                          -Wl,-rpath -Wl,${pin_path}/extras/${xed2_dir}/lib \
                          -L${pin_base_dir}/lib -L${pin_base_dir}/lib-ext \
                          -L${pin_path}/extras/${xed2_dir}/lib"])
            AC_SUBST(PIN_LIBRARIES,
                        ["-lpin -lxed -ldwarf -lelf -ldl"])
            AC_SUBST(XED2_PATH,
            			["${pin_path}/extras/${xed2_dir}"])
            AC_SUBST(PIN_INCLUDES,
                        ["${pin_path}/source/include/pin ${pin_path}/source/include/pin/gen \
                          ${pin_path}/extras/components/include \
                          ${pin_path}/extras/${xed2_dir}/include"])
            ac_have_pin="yes"
        fi
    else
        AC_MSG_NOTICE([PIN tools have been disabled])
    fi
    AM_CONDITIONAL([HAVE_PIN], [test "x$ac_have_pin" = "xyes"])
])

