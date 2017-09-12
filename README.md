# STOMT Unreal-SDK [![Stomt API](https://img.shields.io/badge/stomt-v2.4.X-brightgreen.svg)](https://rest.stomt.com/)

<img alt="STOMT Unreal Engine feedback integration" src="http://schukies.io/images/stomt/StomtUnrealWidget_5-2017.PNG" />

This SDK allows the easy integration of the feedback solution [www.stomt.com](https://www.stomt.com/) in your Unreal apps and games.

## Why?

STOMT allows you to collect simple and constructive feedback in-game and [also from your website](https://github.com/stomt/stomt-javascript-sdk) and collect it at one central place where you manage it and where your community can vote and comment on the feedback of others. (see use-cases below)

## Features

* Collect constructive feedback in-game
* Append error-logs
* Label the feedback automatically (e.g. with the game version number)
* All feedback is collected at your STOMT profile ([Example](https://www.stomt.com/empires-of-the-undergrowth)) where you can manage and react to the feedback
* Login with a STOMT account or anonymously
* Leave EMail after writing a stomt
* Persistent Login across all unreal games


## Some Use-Cases // What will the result look like? // Example Games that use our widget

[Empires of the Undergrowth](https://www.stomt.com/empires-of-the-undergrowth)      
[All Walls Must Fall](https://www.stomt.com/AWMF)

## Installation

This repository contains a complete Unreal-Project with the stomt plugin already integrated.
You can integrate it into your game by following the steps below.
If you use an older unreal engine version please check out the UE/[version] branch.

1. Register on [www.stomt.com](https://www.stomt.com/) 
2. [Create a target page on STOMT](https://www.stomt.com/createTarget).
3. Go to [Settings > My Apps](https://www.stomt.com/dev/my-apps) and create an application.
4. Clone Or Download Zip and extract this repository to a folder.
5. Copy the Plugins folder from the repository into your project directory. 
7. IF you do not have c++ code, use the Add New button in the editor and add a blank c++ class to your project.
6. Add the StomtPlugin to your projects PublicDependencyModuleNames in the projects build.cs ```/Source/ProjectName/ProjectName.Build.cs``` if you have c++ code included.
8. Copy the repository```/Content/Stomt``` folder into your content folder.
9. Place the StomtActor in your scene and press play. (Press 'E' to toggle the widget on/off)
10. Enter the AppId you obtained in the second step and your target username into the ```StomtWidgetBP``` Blueprint.

You can enter the configuration directly into the blueprint or access the variables.

<img alt="STOMT Plugin configuraton" src="http://schukies.io/images/stomt/enter-config.PNG" />

## Use our Sandbox!
If you want to test the integration please feel free to do what you want on [test.stomt.com](https://test.stomt.com/) 

* Just go through the installation steps again on [test.stomt.com](https://test.stomt.com/)
* Enter "https://test.rest.stomt.com" as Rest Server URL in the widget.

## Events

The STOMT Widget supports a variety of callback events.

This shows how you can access them:

<img alt="Events" src="http://schukies.io/images/stomt/example.PNG" />


## Common Issues

* none

Don't hesitate to [contact](https://www.stomt.com/stomt-unreal-engine-plugin) us if you have any problems or need help.

## Contribution

We would love to see you contributing to this project. Feel free to fork it and send in your pull requests! Visit the [project on STOMT](https://www.stomt.com/stomt-unreal-engine-plugin) to support with your ideas, wishes and feedback.

## Authors

[Daniel Schukies](https://github.com/daniel-schukies) | [Follow me on STOMT](https://www.stomt.com/danielschukies)    

## More about stomt

* On the web [www.stomt.com](https://www.stomt.com)
* [STOMT for iOS](http://stomt.co/ios)
* [STOMT for Android](http://stomt.co/android)
