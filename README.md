# STOMT Unreal-SDK [![Stomt API](https://img.shields.io/badge/stomt-v2.4.X-brightgreen.svg)](https://rest.stomt.com/)

<img alt="STOMT Unreal Engine feedback integration" src="http://schukies.io/images/stomt/StomtUnrealWidget_5-2017.PNG" />

This SDK allows the easy integration of the feedback solution [www.stomt.com](https://www.stomt.com/) in your Unreal apps and games.

## Why?

STOMT allows you to collect feedback in-game and [also from your website](https://github.com/stomt/stomt-javascript-sdk) and collect it at one central place where you manage it and where your community can vote and comment on the feedback of others. (see use-cases below)

## Use-Cases // Games that use our widget

[Empires of the Undergrowth](https://www.stomt.com/empires-of-the-undergrowth)      
[All Walls Must Fall](https://www.stomt.com/AWMF)

## Installation

1. Register on [www.stomt.com](https://www.stomt.com/) 
2. [Create a target page on STOMT](https://www.stomt.com/createTarget).
3. Go to [Settings > My Apps](https://www.stomt.com/dev/my-apps) and create an application.
4. Clone Or Download Zip and extract this repository to a folder.
5. Copy the Plugins folder into your project directory. 
6. Add the StomtPlugin to your projects PublicDependencyModuleNames in the projects build.cs if you have c++ code included.
7. IF you do not have c++ code, use the Add New button in the editor and add a blank c++ class to your project.
8. Copy the ProjectName/Content/Stomt folder into your content folder.
9. Place the StomtActor in your scene and press play.
10. Enter the AppId you obtained in the second step and your target username into the ```StomtWidgetBP``` Blueprint.

## Use our Sandbox!
If you want to test the integration please feel free to do what you want on [test.stomt.com](https://test.stomt.com/) 

* Just go through the installation steps again on [test.stomt.com](https://test.stomt.com/)
* Enter "https://test.rest.stomt.com" as Rest Server URL in the widget.

## Common Issues

* none

## Contribution

We would love to see you contributing to this project. Feel free to fork it and send in your pull requests! Visit the [project on STOMT](https://www.stomt.com/) to support with your ideas, wishes and feedback.

## Authors

[Daniel Schukies](https://github.com/daniel-schukies) | [Follow me on STOMT](https://www.stomt.com/danielschukies)    

## More about stomt

* On the web [www.stomt.com](https://www.stomt.com)
* [STOMT for iOS](http://stomt.co/ios)
* [STOMT for Android](http://stomt.co/android)
