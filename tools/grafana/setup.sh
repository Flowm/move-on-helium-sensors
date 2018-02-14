#!/usr/bin/env bash
set -eux

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

cp $DIR/config/etc/telegraf/telegraf.d/*.conf /etc/telegraf/telegraf.d/
systemctl restart telegraf

influx -execute 'CREATE RETENTION POLICY "moveon" ON telegraf DURATION 14d REPLICATION 1 default'

cd $DIR/ser2mqtt && make install
