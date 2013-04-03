#!/bin/sh

for i in i18n.source/tr_*.ts; do 
	lupdate src qml -ts $i
done

