# OpenKJ Standalone Request Server

Standalone basic single-venue request server implementation for use with [OpenKJ](https://openkj.org/).

This began as a fork of the freely-provided *(thank you!)* original [OpenKJ StandaloneRequestServer](https://github.com/OpenKJ/StandaloneRequestServer), last updated Dec 21, 2018 (as of when I forked it in Jan 2023).

## What's new

Enhancements & changes:

- Search & song requests all handled in one page via active search & request modal dialog
- Search queries sanitized just a little more to reduce queries with excessive whitespace
- Uses [new.css](https://newcss.net/) for a lightweight style base, dark mode enforced
- Uses [htmx](https://htmx.org/) for the in-line active search & song request modal dialog
- Fonts changed to my preferences, with a little extra CSS & JS as needed

## Use & development

I'll repeat what the original README said:

> This is intended for people who already know how to configure and manage their own webservers and have a general familiarity with php. The easier and more feature rich option is to use the hosted service available at [okjsongbook.com](https://okjsongbook.com)

### Running Standalone Request Server

Requirements:

- php
- you can use php's built in web server, or a web server with php support caddy, nginx, apache
- `settings.inc` should be edited with an appropriate database path that the webserver has write access to. If the database file doesn't exist, it will be created automatically.
- You probably also want to change the `$venueName` in `settings.inc` to personalize your instance

### Configuring OpenKJ

Under Tools > Settings > Network, you need to set the Server URL.

Example: If you were serving this from a web server as `http://10.0.0.1/requestserver` you would configure the server URL in OpenKJ to point to `http://10.0.0.1/requestserver/api.php`

NOTE: Standalone Request Server ignores any API key specified in the OpenKJ, so you can leave that blank.

## Docker

A docker compose file is provided for development. Your db dir (by default `./okjweb`) must have group ownership set to `www-data`
