# OpenKJ Standalone Request Server

Standalone basic single-venue request server implementation for use with [OpenKJ](https://openkj.org/).

This began as a fork of the freely-provided *(thank you!)* original [OpenKJ StandaloneRequestServer](https://github.com/OpenKJ/StandaloneRequestServer), last updated Dec 21, 2018 (when I forked it in Jan 2023).

## What's new

Enhancements & changes:

- Single page for active search & song requests via modal
- Search available at all times, even when requests are closed
- Search queries sanitized more to reduce queries with excessive whitespace
- [new.css](https://newcss.net/) for a lightweight css base, dark mode enforced
- [htmx](https://htmx.org/) for active search & request modal
- Fonts changed to my preferences, with a little extra CSS & JS as needed

## Usage

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

## Development

### Docker

A docker compose file is provided for development. Your db dir (by default `./okjweb`) must have group ownership set to `www-data`

### Assets (js & css)

Because I like needlessly optimizing things, the css & js assets are optimized to `css/style.min.css` and `js/script.min.js`.

If you want to make changes, you have two options:

#### 1. Use the un-optimized versions

You can modify the siteheader in `global.inc` to comment out the optimized asset tags and uncomment the un-optimized asset tags. Then just modify `css/venuestyle.css` to suit your style. (You probably don't need to modify `js/script.js`, and if you don't want to, you can keep using the optimized version in `global.inc`)

#### 2. Use node tools

You can do an `npm i` in the project root to install the required node dependencies, and then run any of these:

- `npm run dev`: starts watching both `css/venuestyle.css` & `js/script.js` for changes, rebuilding `css/style.min.css` & `js/script.min.js` as needed
- `npm run build`: does a one-time production build of `css/style.min.css` & `js/script.min.js`
- `npm run watch_css`: watch only `css/venuestyle.css`, rebuilding `css/style.min.css` as needed
- `npm run watch_js`: watch only `js/script.js`, rebuilding `js/script.min.js` as needed
- `npm run build_css`: does a one-time production build of `css/style.min.css`
- `npm run build_js`: does a one-time production build of `js/script.min.js`
