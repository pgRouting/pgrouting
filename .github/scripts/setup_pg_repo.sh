#!/bin/bash
# This file is part of the pgRouting project.
# Copyright (c) 2021-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE

# Set up the PostgreSQL APT repository
# Usage: setup_pg_repo.sh [--testing]
#   --testing  use the *-pgdg-testing suite (pre-release versions)

set -euo pipefail

# Raise priority for apt.postgresql.org
cat << EOF >> ./pgdg.pref
Package: *
Pin: release o=apt.postgresql.org
Pin-Priority: 600
EOF
sudo mv ./pgdg.pref /etc/apt/preferences.d/
sudo apt update

TESTING=""
for arg in "$@"; do
  case "$arg" in
    --testing) TESTING="-t" ;;
    *) echo "Unknown option: $arg" >&2; exit 1 ;;
  esac
done

sudo apt-get -y purge postgresql-* || true
sudo apt-get install -y postgresql-common ca-certificates
if [ -n "$TESTING" ]; then
  sudo /usr/share/postgresql-common/pgdg/apt.postgresql.org.sh -t -y
else
  sudo /usr/share/postgresql-common/pgdg/apt.postgresql.org.sh -y
fi
