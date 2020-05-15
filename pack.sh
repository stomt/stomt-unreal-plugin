# next (4.20)
git add .
git stash
git checkout UE/4.20

# move root files
mv README.md Docs/README.md
mv CONTRIBUTING.md Docs/CONTRIBUTING.md

# zip
rm ../stomt-unreal-plugin-4-20.zip
zip -r -X ../stomt-unreal-plugin-4-20.zip . -x *.git* -x pack.sh -x *.DS_Store* -x LICENSE.md -x Binaries/ -x Intermediate/ -x Binaries/**\* -x Intermediate/**\*


# next (4.21)
git add .
git stash
git checkout UE/4.21

# move root files
mv README.md Docs/README.md
mv CONTRIBUTING.md Docs/CONTRIBUTING.md

# zip
rm ../stomt-unreal-plugin-4-21.zip
zip -r -X ../stomt-unreal-plugin-4-21.zip . -x *.git* -x pack.sh -x *.DS_Store* -x LICENSE.md -x Binaries/ -x Intermediate/ -x Binaries/**\* -x Intermediate/**\*


# next (4.22)
git add .
git stash
git checkout UE/4.22

# move root files
mv README.md Docs/README.md
mv CONTRIBUTING.md Docs/CONTRIBUTING.md

# zip
rm ../stomt-unreal-plugin-4-22.zip
zip -r -X ../stomt-unreal-plugin-4-22.zip . -x *.git* -x pack.sh -x *.DS_Store* -x LICENSE.md -x Binaries/ -x Intermediate/ -x Binaries/**\* -x Intermediate/**\*


# next (4.23)
git add .
git stash
git checkout UE/4.23

# move root files
mv README.md Docs/README.md
mv CONTRIBUTING.md Docs/CONTRIBUTING.md

# zip
rm ../stomt-unreal-plugin-4-23.zip
zip -r -X ../stomt-unreal-plugin-4-23.zip . -x *.git* -x pack.sh -x *.DS_Store* -x LICENSE.md -x Binaries/ -x Intermediate/ -x Binaries/**\* -x Intermediate/**\*


# next (4.24)
git add .
git stash
git checkout UE/4.24

# move root files
mv README.md Docs/README.md
mv CONTRIBUTING.md Docs/CONTRIBUTING.md

# zip
rm ../stomt-unreal-plugin-4-24.zip
zip -r -X ../stomt-unreal-plugin-4-24.zip . -x *.git* -x pack.sh -x *.DS_Store* -x LICENSE.md -x Binaries/ -x Intermediate/ -x Binaries/**\* -x Intermediate/**\*


# next (4.25)
git add .
git stash
git checkout UE/4.25

# move root files
mv README.md Docs/README.md
mv CONTRIBUTING.md Docs/CONTRIBUTING.md

# zip
rm ../stomt-unreal-plugin-4-25.zip
zip -r -X ../stomt-unreal-plugin-4-25.zip . -x *.git* -x pack.sh -x *.DS_Store* -x LICENSE.md -x Binaries/ -x Intermediate/ -x Binaries/**\* -x Intermediate/**\*


# clean
git add .
git stash
git checkout master
