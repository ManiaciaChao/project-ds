import { IpcRenderer } from "electron";
import { ICell, CellStatus } from "../redux/store";
import produce from "immer";
import { idToPos } from "../utils";

export const ipcRenderer: IpcRenderer = window.require("electron").ipcRenderer;

const mapNumberToCells = (ele: number): ICell => ({
  status: ele,
  mutable: ele == CellStatus.Unknown,
});

const mapCellsToString = (order: number, cells: ICell[]): string => {
  const lines: string[] = [];
  lines.push(`${order}`);
  cells.forEach((cell, id) => {
    const { x, y } = idToPos(id, order);
    lines.push(`${x} ${y} ${cell.status}`);
  });
  return lines.join("\n");
};

export const generate = (order: number) => {
  const promise = new Promise<ICell[]>((resolve, reject) => {
    ipcRenderer.once("generate-reply", (event, data: string) => {
      try {
        const rawData: number[] = JSON.parse(data);
        resolve(rawData.map(mapNumberToCells));
      } catch (e) {
        reject(e);
      }
    });
  });
  ipcRenderer.send("generate", order);
  return promise;
};

export const solve = (order: number, cells: ICell[]) => {
  const promise = new Promise<boolean>((resolve, reject) => {
    ipcRenderer.once("solve-reply", (event, data: string) => {
      try {
        const rawData: boolean = JSON.parse(data);
        resolve(rawData);
      } catch (e) {
        reject(e);
      }
    });
  });
  ipcRenderer.send("solve", mapCellsToString(order, cells));
  return promise;
};
