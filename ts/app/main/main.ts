const { format } = require("url");

import { BrowserWindow, app } from "electron";
const isDev = require("electron-is-dev");
const { resolve } = require("app-root-path");
import { exec as originExec } from "child_process";
import { promisify } from "util";
import { ipcMain } from "electron";

import { env } from 'process';

const exec = promisify(originExec);

const binPuzzle = env['BIN_PATH'] ?? '../bin_puzzle'

app.on("ready", async () => {
  const mainWindow = new BrowserWindow({
    width: 800,
    height: 600,
    show: false,
    webPreferences: {
      nodeIntegration: true,
      // preload: __dirname + '/preload.js'
    },
  });

  mainWindow.once("ready-to-show", () => {
    mainWindow.show();
    if (isDev) {
      mainWindow.webContents.openDevTools();
    }
  });

  const devPath = "http://localhost:1124";
  const prodPath = format({
    pathname: resolve("app/renderer/.parcel/production/index.html"),
    protocol: "file:",
    slashes: true,
  });
  const url = isDev ? devPath : prodPath;

  mainWindow.setMenu(null);
  mainWindow.loadURL(url);
});

ipcMain.on("generate", (event, order: number) => {
  exec(`${binPuzzle} -n ${order} -g 0`)
    .then((data) => event.reply("generate-reply", data.stdout))
    .catch((err) => console.log(err));
});

ipcMain.on("solve", (event, str:string) => {
  exec(`${binPuzzle} -s ${str}`)
    .then((data) => event.reply("solve-reply", data.stdout))
    .catch((err) => console.log(err));
});

app.on("window-all-closed", app.quit);
