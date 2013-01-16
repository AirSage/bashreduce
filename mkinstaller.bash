#!/bin/bash

rev=$(git describe --long HEAD 2>/dev/null) || \
  ( echo "ERROR: Can't get the  revision number " ; exit 1 )

mksxb -p bashreduce-$rev \
    -a var/bashreduce/install/initialsetup.bash \
    var/bashreduce/install/mksxb.cfg

scp bashreduce*-$rev.sxb root@10.255.3.20:/opt/server/lighttpd/pkg/

