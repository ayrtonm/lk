echo 'Copying template/ to '$1'/'
cp -r template $1
cd $1
sed -i 's/template/'$1'/g' *
mv template.c $1.c
cd ..
