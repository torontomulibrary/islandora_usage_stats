# Islandora Usage Stats

## Introduction

A module for Drupal 7 to track views and downloads of Islandora items.

Work based on code from https://github.com/roblib/islandora_scholar_upei and the scholar_tracking module for Drupal 6. Iterated on by the Ryerson Digital Repository and discoverygarden inc.

## Requirements

This module requires the following modules/libraries:

* [Tuque](https://github.com/islandora/tuque)
* [Islandora](https://github.com/islandora/islandora)
* [Islandora basic collection](https://github.com/Islandora/islandora_solution_pack_collection)
* [Views (3.x)](https://www.drupal.org/project/views)
* [Datepicker](https://www.drupal.org/project/datepicker)

## Configuration

Configuration options available at `admin/islandora/tools/islandora_usage_stats`.

## Features

* Toggle to ignore common bots
* View count uses session variables and defaults to a 5 minute cooldown for repeated requests
* Access log for all views and downloads
* IP Exclusion list to prevent artificially inflating counts while testing/developing/administrating
* Several customizable blocks to display metrics
* Report generating interface
* Object log views integration

## Notes

* Does not respect XACML.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
