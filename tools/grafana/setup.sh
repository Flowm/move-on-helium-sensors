#!/usr/bin/env bash
set -eux

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Telegraf
cp $DIR/config/etc/telegraf/telegraf.d/*.conf /etc/telegraf/telegraf.d/
systemctl restart telegraf

# Influxdb
influx -execute 'CREATE RETENTION POLICY "moveon" ON telegraf DURATION 14d REPLICATION 1 default'
influx -port 48086 -execute 'CREATE RETENTION POLICY "moveon" ON telegraf DURATION 28d REPLICATION 1 default'

# Bin
cd $DIR/bin/mqttserial && make install

# Daemon
$DIR/daemon/ser2mqtt/setup.sh
