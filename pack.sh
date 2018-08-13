# open
git add .
git stash
git checkout UE/4.18

# move root files
mv README.md Docs/README.md
mv CONTRIBUTING.md Docs/CONTRIBUTING.md

# zip
rm ../stomt-unreal-plugin-4-18.zip
zip -r -X ../stomt-unreal-plugin-4-18.zip . -x *.git* -x pack.sh -x *.DS_Store* -x LICENSE.md

# next
git add .
git stash
git checkout UE/4.19

# move root files
mv README.md Docs/README.md
mv CONTRIBUTING.md Docs/CONTRIBUTING.md

# zip
rm ../stomt-unreal-plugin-4-19.zip
zip -r -X ../stomt-unreal-plugin-4-19.zip . -x *.git* -x pack.sh -x *.DS_Store* -x LICENSE.md

# next
git add .
git stash
git checkout UE/4.20

# move root files
mv README.md Docs/README.md
mv CONTRIBUTING.md Docs/CONTRIBUTING.md

# zip
rm ../stomt-unreal-plugin-4-20.zip
zip -r -X ../stomt-unreal-plugin-4-20.zip . -x *.git* -x pack.sh -x *.DS_Store* -x LICENSE.md

# clean
git add .
git stash
git checkout master
