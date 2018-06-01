#!/bin/bash

set -euo pipefail

test -f VERSION
VERSION=$(cat VERSION)

if [[ -d .git ]]; then
	if git describe --tags >/dev/null 2>&1; then
		VERSION_GIT=$(git describe --tags --always --dirty)

		# Remove leading 'v'
		VERSION_GIT=${VERSION_GIT#v}

		if [[ $VERSION_GIT == ${VERSION}* ]]; then
			VERSION=$VERSION_GIT
		else
			echo "$(tput setaf 1)ERROR:$(tput sgr0) Git version '$VERSION_GIT' does not match '$VERSION' in VERSION file!" >&2
			exit 1
		fi
	else
		VERSION=$VERSION-g$(git describe --always --dirty)
	fi
fi

echo $VERSION
