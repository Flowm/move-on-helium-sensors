#!/usr/bin/env bash
set -eux

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Telegraf
cp $DIR/config/etc/telegraf/telegraf.d/*.conf /etc/telegraf/telegraf.d/
systemctl restart telegraf

# Influxdb
influx -execute 'CREATE RETENTION POLICY "moveon" ON telegraf DURATION 14d REPLICATION 1 default'

# Bin
cd $DIR/bin/mqttserial && make install

# Daemon
cd $DIR/daemon
for srv in *; do
    if ![ -f $srv/$srv.service]; then
        continue
    fi
    cp $srv/$srv.service /etc/systemd/system/
    systemctl daemon-reload
    systemctl enable $srv
    systemctl restart $srv
done
