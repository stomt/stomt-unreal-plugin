# Collect Feedback In-Game | STOMT for Unreal Engine 4
#### Implementation Time: ~20 Minutes (incl. Triggers)
#### Base Unreal Version: 4.18 (For specific versions visit: [4.16](https://github.com/stomt/stomt-unreal-plugin/tree/UE/4.16), [4.17](https://github.com/stomt/stomt-unreal-plugin/tree/UE/4.17), [4.18](https://github.com/stomt/stomt-unreal-plugin/tree/UE/4.18), [4.19](https://github.com/stomt/stomt-unreal-plugin/tree/UE/4.19), [4.20](https://github.com/stomt/stomt-unreal-plugin/tree/UE/4.20), [4.21](https://github.com/stomt/stomt-unreal-plugin/tree/UE/4.21))

<p align="center">
  <img alt="STOMT Unreal Engine feedback integration" src="https://i.imgur.com/1i1eCrb.jpg" />
</p>

This SDK allows the easy integration of the feedback solution [www.stomt.com](https://www.stomt.com/) in your Unreal apps and games.


## Use-Cases

Example Games that use our integrations:

* [Empires of the Undergrowth](https://www.stomt.com/empires-of-the-undergrowth)
* [All Walls Must Fall](https://www.stomt.com/AWMF)
* [Pantropy](https://www.stomt.com/pantropy)


## Installation

### Installation via Marketplace (recommended)

1. [Download](com.epicgames.launcher://ue/marketplace/content/7980672d57664bb5a567ff39f5106af6) the Plugin via the [Unreal Engine Marketplace](https://www.unrealengine.com/marketplace/stomt-collect-feedback-community-building)

2. Enable StomtPlugin in your Plugins Window (in Unreal Editor: `Edit -> Plugins -> Installed -> Widgets -> StomtPlugin`)

<img alt="Enable StomtPlugin in Unreal Editor" width="600" src="https://github.com/stomt/stomt-unreal-plugin/raw/master/Docs/Images/unreal_enable_StomtPlugin.png" />

3. Show the Engine Content in your Content Browser

<img alt="Unhide the Engine Content in Unreal Editor" width="600" src="https://github.com/stomt/stomt-unreal-plugin/raw/master/Docs/Images/unreal_StomtPlugin_enable_engine_content.png" />

4. Find StomtPlugin Content

<img alt="Discover StomtPlugins Conent" width="600" src="https://github.com/stomt/stomt-unreal-plugin/raw/master/Docs/Images/unreal_StomtPlugin_find_widget_content.png" />

5. Place the `ExampleActor` in your scene.

6. Click play to run the game and show the feedback widget by pressing `E`.

_You should now see the feedback form window in your game. Try to toggle "I wish"/"I like" and send a test stomt to "My Game" (The default test target). Then continue with [Configuration](#configuration)._

### Manual Installation

_If you use an older Unreal Engine version please check out the [`UE/[version]` branch](https://github.com/stomt/stomt-unreal-plugin/branches)._

1. Clone or download this repository into your projects `Plugins` directory.

1.2. **If you do not use C++ code in project:** use the `Add New` button in the editor and add a blank C++ class to your project.
[Guide](https://docs.unrealengine.com/latest/INT/Programming/QuickStart/2/index.html)

2. Add the StomtPlugin to your projects `PublicDependencyModuleNames` in the projects `build.cs`. `/Source/[YourProjectName]/[YourProjectName].Build.cs`:
```c++
PublicDependencyModuleNames.AddRange(new string[] { "StomtPlugin" }); // Add "StomtPlugin" String
```

3. Restart Unreal Editor, open your project and enable the plugin. `Edit -> Plugins -> Widgets -> StomtPlugin`

4. Check `Show Plugin Content` and `Show Engine Content` in the bottom right corner of your editor (view options).

<img alt="Events" width="600" src="https://i.imgur.com/YoTY7w6.png" />

5. Place the `ExampleActor` in your scene.

6. Click play to run the game and show the feedback widget by pressing `E`.

_You should now see the feedback form window in your game. Try to toggle "I wish"/"I like" and send a test stomt to "My Game" (The default test target). Then continue with [Configuration](#configuration)._


## Configuration

1. Create a page for your game on [www.stomt.com](https://www.stomt.com/signup/game).

2. Create an [App Id](https://www.stomt.com/integrate) for Unreal.

3. Enter the `App Id` into the `ExampleActor` Blueprint:

<img alt="Unhide the Engine Content in Unreal Editor" width="600" src="https://github.com/stomt/stomt-unreal-plugin/raw/master/Docs/Images/unreal_StomtPlugin_configure_AppID.png" />

**You can disable the screenshot and log-upload functionality**

<img alt="STOMT Plugin configuraton" width="600" src="https://i.imgur.com/hFA7xUs.png" />


4. Add the `StomtWidgetBP` to the viewport via script (`ExampleActor`) or in your main HUD (`ExampleUI`) as widget.





## Form Triggers

The widget can be opened and closed whenever you want by using our trigger functions.     
    
That allows you to:    
* Put a button into the main menu [(Example)](https://imgur.com/5SoQzfj)
* Put a button into the HUD [(Example)](https://imgur.com/t9wPpJj)
* Only show the button to certain players (e.g. power users)
* Trigger the form after certain events

**Define a hotkey:**

<img alt="STOMT Plugin Form Triggers" width="600" src="https://i.imgur.com/HVhJXq4.png" />


## Event-Callbacks

The STOMT Widget supports a variety of callback events.

<img alt="Events" width="600" src="http://schukies.io/images/stomt/example.PNG" />

## In-Game Labeling

Labels will help you track down user issues.
Append labels, as for example your game-version or the player position/level. You can either hardcode them in the Unity Inspector or use a script to add them in a flexible way based on the information you have.    

<p align="center">
<img alt="Events" title="In-Game Feedback Tagging" width="600" height="auto" src="https://i.imgur.com/sS8T8Fy.png" />
</p>

**Easily add an array of labels**

<img alt="Events" width="600" src="http://schukies.io/images/stomt/label.PNG" />

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
