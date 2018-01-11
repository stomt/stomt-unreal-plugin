# Collect Feedback In-Game | STOMT for Unreal Engine 4
#### Implementation Time: ~20 Minutes (incl. Triggers)

<p align="center">
  <img alt="STOMT Unreal Engine feedback integration" src="https://i.imgur.com/hK8BRqq.jpg" />
</p>

This SDK allows the easy integration of the feedback solution [www.stomt.com](https://www.stomt.com/) in your Unreal apps and games.

[Unreal Engine Wiki Page](https://wiki.unrealengine.com/STOMT_Plugin)

## Why?

STOMT allows you to collect simple and constructive feedback in-game and [also from your website](https://stomt.co/web) and collect it at one central place where you manage it and where your community can vote and comment on the feedback of others. (see [use cases](#use-cases) below)


## Features

* Collect constructive feedback in-game
* Append error-logs
* Label the feedback automatically (e.g. with the game version number)
* All feedback is collected at your STOMT profile ([Example](https://www.stomt.com/empires-of-the-undergrowth)) where you can manage and react to the feedback
* Login with a STOMT account or anonymously
* Leave E-Mail after writing a stomt
* Persistent Login across all Unreal games


## Use-Cases

_What will the result look like?_ 

Example Games that use our widget:

* [Empires of the Undergrowth](https://www.stomt.com/empires-of-the-undergrowth)      
* [All Walls Must Fall](https://www.stomt.com/AWMF)


## Installation

Integrate the plugin into your game by following the steps below.

_If you use an older Unreal Engine version please check out the `UE/[version]` branch._

1. Clone or download this repository into your projects `Plugins` directory.

2. **If you do not use C++ code in project:** use the `Add New` button in the editor and add a blank C++ class to your project.
[Guide](https://docs.unrealengine.com/latest/INT/Programming/QuickStart/2/index.html)

3. Add the StomtPlugin to your projects `PublicDependencyModuleNames` in the projects `build.cs`. `/Source/[YourProjectName]/[YourProjectName].Build.cs`:
```c++
PublicDependencyModuleNames.AddRange(new string[] { "StomtPlugin" }); // Add "StomtPlugin" String
```

4. Open your project with the unreal editor and enable the plugin. `Edit -> Plugins -> Widgets -> STOMT Plugin`

5. Check `Show Plugin Content` in the bottom right corner of your editor (view options).

<img alt="Events" src="https://i.imgur.com/YoTY7w6.png" />

6. Place the `ExampleActor` in your scene and press play. (Press 'E' to toggle the widget on/off)



## Configuration

1. Register on [www.stomt.com](https://www.stomt.com/signup/game) 

2. And create an [App Id](https://www.stomt.com/integrate) for your project.

3. Enter the variables into the `StomtWidgetBP` Blueprint:

* the `App Id` you obtained in the second step
* the `Target Id` of your STOMT project (stomt.com/{Target Id}
* the production `Rest URL`: `https://rest.stomt.com`

You can enter the configuration directly into the `StomtWidgetBP` blueprint or access the variables.

<img alt="STOMT Plugin configuraton" src="http://schukies.io/images/stomt/enter-config.PNG" />

<img alt="STOMT Plugin configuraton" src="https://i.imgur.com/RkZtEaR.jpg" />


## Events

The STOMT Widget supports a variety of callback events.

This shows how you can access them:

<img alt="Events" src="http://schukies.io/images/stomt/example.PNG" />


## Labels

Labels will help you track down user issues.
Append labels, as for example your game-version or the player position.
<p align="center">
<img alt="Events" src="https://i.imgur.com/sS8T8Fy.png" />
</p>

<img alt="Events" src="http://schukies.io/images/stomt/label.PNG" />

## Common Issues

* UE 4.18.2 Error/Info message text disappears when it's displayed for the second time.
* UE 4.16.3 None

Don't hesitate to [contact](https://www.stomt.com/stomt-unreal-engine-plugin) us if you have any issues or need help.


## Contribution

We would love to see you contributing to this project. Feel free to fork it and send in your pull requests! Visit the [project on STOMT](https://www.stomt.com/stomt-unreal-engine-plugin) to support with your ideas, wishes and feedback.


## Authors

[Daniel Schukies](https://github.com/daniel-schukies) | [Follow me on STOMT](https://www.stomt.com/danielschukies)    


## More about stomt

* On the web [www.stomt.com](https://www.stomt.com)
* [STOMT for iOS](http://stomt.co/ios)
* [STOMT for Android](http://stomt.co/android)
* [STOMT for Unity](http://stomt.co/unity)
* [STOMT for Websites](http://stomt.co/web)
* [STOMT for Wordpress](http://stomt.co/wordpress)
* [STOMT for Drupal](http://stomt.co/drupal)
