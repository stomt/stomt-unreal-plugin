git checkout develop

# mergin in master
git checkout master
git merge develop

# merge in version branches
git checkout UE/4.20
git merge develop

git checkout UE/4.21
git merge develop

git checkout UE/4.22
git merge develop

git checkout UE/4.23
git merge develop

git checkout UE/4.24
git merge develop

git checkout UE/4.25
git merge develop
