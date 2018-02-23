#!/usr/bin/env bash
set -eux

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR

for srv in *; do
    if ! [ -f "$srv/$srv.service" ]; then
        continue
    fi
    cp $srv/$srv.service /etc/systemd/system/ || true
    sudo systemctl daemon-reload || true
    sudo systemctl enable $srv || true
    sudo systemctl restart $srv
done
