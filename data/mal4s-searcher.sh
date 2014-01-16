#!/bin/bash
# PURPOSE: easily spin through a file 
# looking at one RIR at a time
# for a given search term or all for that term
# $1 is egrep style what to search the data for

# FIXME DAY should be configurable possibly an arg
DAY=`date -v-1d +%F`

# The file name you want to retrieve every day
FNAME="${DAY}_regdate--newns.mal4s"

# The path where you want to store downloaded Mal4s files
FDIR="/Users/YOURUSERHERE/Downloads"

# The URL to download the *.mal4s data file from
URL="http://YOURASSIGNEDHOST.dc-stack.us/mal4s"

# Full path and name of the Mal4s configuration files to use
TEXTCONF="/opt/mal4s/data/newns.conf"
MAINCONF="/opt/mal4s/data/dissect-local.conf"

# Terms to exclude in the search such as lines you DO NOT want in the data
# FIXME should come from $2 but default to value specified here
# FIXME need a no-effect default if no exclusion is desired
EXCLUDE="ORDERBOX-DNS"

# ****************** END OF USER CONFIGURATION

# Get the file if we don't have it:

if [ -e ${FDIR}/${FNAME} ];then
	ls -lthr ${FDIR}/${FNAME}
else
	wget -O ${FDIR}/${FNAME} ${URL}/${FNAME}
fi

ls -lthr ${FDIR}/${FNAME}

# Decide based on count of results for search terms
# if we should display all on one screen or loop thru

SEARCHCOUNT=`grep -v "${EXCLUDE}" ${FDIR}/${FNAME}|egrep -c ${1}`

if [ "${SEARCHCOUNT}" -lt "501" ];then
	egrep "${1}" ${FDIR}/${FNAME} |\
	grep -v "${EXCLUDE}" |\
	mal4s --load-config ${MAINCONF} --load-text-config ${TEXTCONF}
else
for i in {ripe,arin,apnic,lacnic,afrinic};do
	grep -F ${i} ${FDIR}/${FNAME} |\
	egrep "${1}" |\
	grep -v "${EXCLUDE}" |\
	mal4s --load-config ${MAINCONF} --load-text-config ${TEXTCONF}
done
fi

