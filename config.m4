PHP_ARG_WITH(aokex, for ao key exchange
[  --with-aokex          Enable aokex support])

if test "$PHP_AOKEX" != "no"; then
  AC_DEFINE(HAVE_AOKEX,1,[ ])
  PHP_EXTENSION(aokex, $ext_shared)
fi
