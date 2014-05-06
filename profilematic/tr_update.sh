#!/bin/sh

for i in i18n.source/tr_*.ts; do 
	lupdate ../profilematicd/src src qml -ts $i
done

