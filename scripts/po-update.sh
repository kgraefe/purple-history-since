#!/bin/bash

set -euo pipefail

cd "$(dirname "$0")/.."

ls -1 src/*.c > po/POTFILES.in
GETTEXT_PKG=$(awk -F= '/GETTEXT_PACKAGE=/ {print $2}' configure.ac)

cd po
intltool-update -pot -gettext-package=$GETTEXT_PKG

sed \
	-i 's#^\"Content-Type: text/plain; charset=CHARSET\\n\"$#"Content-Type: text/plain; charset=UTF-8\\n"#g' \
	${GETTEXT_PKG}.pot

