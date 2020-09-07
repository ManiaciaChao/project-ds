import { Reducer, ReducerFromReducersMapObject } from "redux";
import { produce } from "immer";
import {
  IGameInfo,
  initialGameInfo,
  IBoard,
  initialBoard,
  initialCell,
  ICell,
} from "./store";
import { GameActionType, BoardActionType } from "./actions";

const generateEmptyCells = (order: number) =>
  Array(order ** 2).fill(initialCell) as ICell[];

export const gameReducer: Reducer<IGameInfo> = (
  state = initialGameInfo,
  action
) => {
  switch (action.type) {
    case GameActionType.Init: {
      return { ...initialGameInfo, startTime: Date.now() };
    }
    case GameActionType.Submit: {
      return state;
    }
    case GameActionType.Difficulty: {
      return { ...state, difficulty: action.data };
    }
    default: {
      return state;
    }
  }
};

export const boardReducer: Reducer<IBoard> = (state = initialBoard, action) => {
  switch (action.type) {
    case BoardActionType.Init: {
      const order = action?.data ?? 4;
      const cells = generateEmptyCells(order);
      return {
        ...initialBoard,
        order,
        cells,
      };
    }
    case BoardActionType.Load: {
      const cells = action.data;
      return { ...state, cells };
    }
    case BoardActionType.Move: {
      const { idx, status } = action.data;
      const cells = [...state.cells];
      cells[idx] = {
        ...cells[idx],
        status,
        lastModifiedTime: Date.now(),
      };
      return { ...state, cells };
    }
    case BoardActionType.Clear: {
      const { order } = state;
      return { ...state, cells: generateEmptyCells(order) };
    }
    default: {
      return state;
    }
  }
};
