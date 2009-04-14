#!/bin/sh

LANG=C

status=0

if ! which readelf 2>/dev/null >/dev/null; then
	echo "'readelf' not found; skipping test"
	exit 0
fi

for so in .libs/lib*.so; do
	echo Checking "$so" for local PLT entries
	readelf -W -r "$so" | grep 'JU\?MP_SLO' | grep 'cairo' && status=1
done

exit $status
