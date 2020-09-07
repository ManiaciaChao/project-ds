import { Action } from "redux";
import { Reducer } from "react";
import { ISudokuStore, CellStatus, Difficulty, ICell } from "./store";

export enum GameActionType {
  Init,
  Submit,
  Difficulty,
}

export enum BoardActionType {
  Init,
  Load,
  Save,
  Move,
  Clear,
}

export const GameActions = {
  setDifficulty: (difficulty: Difficulty) => ({
    type: GameActionType.Difficulty,
    data: difficulty,
  }),
};

export const BoardActions = {
  init: (order: number) => ({
    type: BoardActionType.Init,
    data: order,
  }),
  makeMove: (idx: number, status: CellStatus) => ({
    type: BoardActionType.Move,
    data: { idx, status },
  }),
  load: (cells:ICell[])=>({
    type: BoardActionType.Load,
    data: cells
  })
};
