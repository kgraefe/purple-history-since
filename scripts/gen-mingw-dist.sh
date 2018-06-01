#!/bin/bash

set -euo pipefail

OUTPUT="$1"
PLUGIN="$2"
PLUGIN_DLL="$3"

TEMPDIR=$(mktemp -d)
trap "rm -rf '$TEMPDIR'" EXIT

mkdir -p $TEMPDIR/$PLUGIN/plugins
cp src/$PLUGIN_DLL $TEMPDIR/$PLUGIN/plugins

if [[ $(uname -s) == CYGWIN* ]]; then
	strip "$(cygpath -w $TEMPDIR/$PLUGIN/plugins/$PLUGIN_DLL)"
else
	strip $TEMPDIR/$PLUGIN/plugins/$PLUGIN_DLL
fi

GETTEXT_PKG=$(awk -F= '/GETTEXT_PACKAGE=/ {print $2}' configure.ac);
for f in po/*.po; do
	[ -f "$f" ] || continue
	lang=$(basename $f .po)
	mkdir -p $TEMPDIR/$PLUGIN/locale/$lang/LC_MESSAGES
	cp po/$lang.gmo $TEMPDIR/$PLUGIN/locale/$lang/LC_MESSAGES/$GETTEXT_PKG.mo
done

for f in README.md CHANGES.md AUTHORS.md COPYING; do
	unix2dos -n $f $TEMPDIR/$f
done

pushd .
cd $TEMPDIR/
zip -r archive.zip .
popd

mv $TEMPDIR/archive.zip "$OUTPUT"
