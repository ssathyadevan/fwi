#/bin/bash
# Modifies the Google Test XMLoutput file to JUnit compatible MXL outputs

GTESTXML=$(find FWIInstall/bin -name "*.xml" -and -not -name "SystemTests.xml") 

for FILES in ${GTESTXML}
do
	FULL_PATH=$(readlink -m ${FILES})
	FULL_DIR=$(dirname ${FULL_PATH})
	filename=$(basename "$FULL_PATH")
	filename="${filename%.*}"
	awk '{ if ($1 == "<testcase" && match($0, "notrun")) print substr($0,0,length($0)-2) "><skipped/></testcase>"; else print $0;}'\
	 ${FILES}> "${FULL_DIR}/${filename}_s.xml"
	mv "${FULL_DIR}/${filename}_s.xml" "${FULL_DIR}/${filename}.xml"
done
