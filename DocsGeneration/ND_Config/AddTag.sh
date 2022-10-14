#!/bin/sh

TAG=$(cat ./GoogleTag.txt)
echo ${TAG}
sed "s/<head>/<head> \n${TAG}/1g" ../../docs/index.html > ../../docs/index_new.html
rm ../../docs/index.html
mv ../../docs/index_new.html ../../docs/index.html
