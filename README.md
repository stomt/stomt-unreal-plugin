# Collect Feedback In-Game | STOMT for Unreal Engine 4
#### Implementation Time: ~20 Minutes (incl. Triggers)
#### Unreal Version: 4.18.2 (There is also a 4.16 branch)

<p align="center">
  <img alt="STOMT Unreal Engine feedback integration" src="https://i.imgur.com/1i1eCrb.jpg" />
</p>

This SDK allows the easy integration of the feedback solution [www.stomt.com](https://www.stomt.com/) in your Unreal apps and games.

[Unreal Engine Wiki Page](https://wiki.unrealengine.com/STOMT_Plugin)

## Examples / Use-Cases / Demo

Example Games that use our integrations:

* [Empires of the Undergrowth](https://www.stomt.com/empires-of-the-undergrowth)      
* [All Walls Must Fall](https://www.stomt.com/AWMF)
* [Pantropy](https://www.stomt.com/pantropy)

## Installation

_If you use an older Unreal Engine version please check out the `UE/[version]` branch._

1. Clone or download this repository into your projects `Plugins` directory.

1.2. **If you do not use C++ code in project:** use the `Add New` button in the editor and add a blank C++ class to your project.
[Guide](https://docs.unrealengine.com/latest/INT/Programming/QuickStart/2/index.html)

2. Add the StomtPlugin to your projects `PublicDependencyModuleNames` in the projects `build.cs`. `/Source/[YourProjectName]/[YourProjectName].Build.cs`:
```c++
PublicDependencyModuleNames.AddRange(new string[] { "StomtPlugin" }); // Add "StomtPlugin" String
```

3. Open your project and enable the plugin. `Edit -> Plugins -> Widgets -> STOMT Plugin`

4. Check `Show Plugin Content` in the bottom right corner of your editor (view options).

<img alt="Events" src="https://i.imgur.com/YoTY7w6.png" />

5. Place the `ExampleActor` in your scene and press play. (Press 'E' to toggle the widget on/off)


## Configuration

1. Create a page for your game on [www.stomt.com](https://www.stomt.com/signup/game).

2. Create an [App Id](https://www.stomt.com/integrate) for Unreal.

3. Enter the variables into the `StomtWidgetBP` Blueprint:

* the `App Id` you obtained in the second step
* the `Target Id` of your STOMT project (stomt.com/{Target Id})
* the production `Rest URL`: `https://rest.stomt.com`

**Enter the configuration directly into the `StomtWidgetBP` blueprint**

<img alt="STOMT Plugin configuraton" src="http://schukies.io/images/stomt/enter-config.PNG" />

**Alternatively access the variables**
<img alt="STOMT Plugin configuraton" src="https://i.imgur.com/RkZtEaR.jpg" />

## Form Triggers

The widget can be opened and closed whenever you want by using our trigger functions.     
    
That allows you to:    
* Put a button into the main menu [(Example)](https://imgur.com/5SoQzfj)
* Put a button into the HUD [(Example)](https://imgur.com/t9wPpJj)
* Only show the button to certain players (e.g. power users)
* Trigger the form after certain events

**Define a hotkey:**
<img alt="STOMT Plugin Form Triggers" src="https://i.imgur.com/HVhJXq4.png" />


## Event-Callbacks

The STOMT Widget supports a variety of callback events.

<img alt="Events" src="http://schukies.io/images/stomt/example.PNG" />

## In-Game Labeling

Labels will help you track down user issues.
Append labels, as for example your game-version or the player position/level. You can either hardcode them in the Unity Inspector or use a script to add them in a flexible way based on the information you have.    

<p align="center">
<img alt="Events" title="In-Game Feedback Tagging" width="600" height="auto" src="https://i.imgur.com/sS8T8Fy.png" />
</p>

**Easily add an array of labels**

<img alt="Events" src="http://schukies.io/images/stomt/label.PNG" />

## Common Issues

* UE 4.18.2 Error/Info message text disappears when it's displayed for the second time.
* UE 4.16.3 None

Don't hesitate to [contact](https://www.stomt.com/stomt-unreal-engine-plugin) us if you have any issues or need help.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/stomt/stomt-unreal-plugin/tags). 

## Contribution

We would love to see you contributing to this project. Please read [CONTRIBUTING.md](https://github.com/stomt/stomt-unreal-plugin/blob/master/CONTRIBUTING.md) for details on our code of conduct, and the process for submitting pull requests to us.    
    
Visit the [project on STOMT](https://www.stomt.com/stomt-unreal-plugin) to support with your ideas, wishes and feedback.

## Authors

[Daniel Schukies](https://github.com/daniel-schukies) | [Follow Daniel Schukies on STOMT](https://www.stomt.com/danielschukies)

See also the list of [contributors](https://github.com/stomt/stomt-unreal-engine-plugin/contributors) who participated in this project.   

## More about stomt

*Regularly communicate your page on social channels and checkout our [Website-Widget](https://www.stomt.com/dev/js-sdk) for your websites to collect feedback from anywhere.*  

* On the web [www.stomt.com](https://www.stomt.com)
* [STOMT for iOS](http://stomt.co/ios)
* [STOMT for Android](http://stomt.co/android)
* [STOMT for Unity](http://stomt.co/unity)
* [STOMT for Websites](http://stomt.co/web)
* [STOMT for Wordpress](http://stomt.co/wordpress)
* [STOMT for Drupal](http://stomt.co/drupal)
