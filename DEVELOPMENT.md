# Development

## Branches

This repository contains a branch for each Engine Version. Only perform changes in one of these branches if the changes are specific for this version. All other changes are made in the `master` - this branch uses our base version `UE/4.20`. 

- UE/4.16 [deprecated]
- UE/4.17 [deprecated]
- UE/4.18 [deprecated]
- UE/4.19 [deprecated]
- UE/4.20 = master
- UE/4.21 = master + changed EngineVersion
- UE/4.22 = master + changed EngineVersion
- UE/4.23 = master + changed EngineVersion
- UE/4.24 = master + changed EngineVersion
- UE/4.25 = master + changed EngineVersion
- UE/4.26 = master + changed EngineVersion

## Release

The following scripts help to perform a release.

1. Ensure that all nessessary changes have been made in the `master` branch. 

  * Increase Version in StomtPlugin.uplugin
  * Update version number in StomtPlugin.uplugin 
  * Update version number in StomtTrack.cpp line 36 
  * commit changes "git commit -am 'bump version to X.X.X"
  * create tag "git tag -a vX.X.X"

2. Use `sh scripts/delete_branches.sh` to remove local changes in other branches.

3. Update the version branches by running `sh scripts/update_branches.sh`. Confirm the merges and merge commits.

4. Push the updated branches to GitHub by running `sh scripts/push_branches.sh`.

5. Now pack the version archives `sh pack.sh` and upload them for the store.
