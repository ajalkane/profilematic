#!/bin/sh

for i in i18n.source/tr_*.ts; do 
	base=$(basename $i .ts)
	lrelease $i -qm i18n/${base}.qm
done

