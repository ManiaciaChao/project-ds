import { createStore, combineReducers } from "redux";
import { gameReducer, boardReducer } from "./reducers";

import { useSelector, TypedUseSelectorHook } from "react-redux";

export enum Difficulty {
  Easy,
  Medium,
  Hard,
  Extream,
}

export enum CellStatus {
  Unknown = -1,
  False = 0,
  True = 1,
}

export interface ICell {
  status: CellStatus;
  mutable?: boolean;
  lastModifiedTime?: number;
}

export const initialCell = {
  status: CellStatus.Unknown,
  mutable: true
};

export interface IBoard {
  order: number;
  cells: ICell[];
}

export const initialBoard = {
  order: 0,
  cells: [],
};

export interface IGameInfo {
  startTime: number;
  difficulty: Difficulty;
  totalSteps: number;
}

export const initialGameInfo: IGameInfo = {
  startTime: 0,
  difficulty: Difficulty.Easy,
  totalSteps: 0,
};

export const sudokuStore = createStore(
  combineReducers({
    info: gameReducer,
    board: boardReducer,
  })
);

type SudokuStore = ReturnType<typeof sudokuStore.getState>;
export const useSudokuSelector: TypedUseSelectorHook<SudokuStore> = useSelector;
