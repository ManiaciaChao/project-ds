import * as React from "react";
import { FC, useState, useEffect } from "react";

import { useSudokuSelector, Difficulty } from "../../models/redux/store";

import { Board } from "../board/board";

import "./styles.less";
import { useDispatch } from "react-redux";
import {
  BoardActions,
  GameActionType,
  GameActions,
} from "../../models/redux/actions";

import { ipcRenderer, generate } from "../../models/ipc/ipcRenderer";

const difficultyPhraseMap = {
  [Difficulty.Easy]: "简单",
  [Difficulty.Medium]: "中等",
  [Difficulty.Hard]: "困难",
  [Difficulty.Extream]: "极限",
};

export const Game: FC = (props) => {
  const { info, order } = useSudokuSelector((state) => ({
    info: state.info,
    order: state.board.order,
  }));
  const [totalSeconds, setTotalSeconds] = useState(0);
  const loadLock = React.useRef(false);
  const dispatch = useDispatch();

  useEffect(() => {
    const handle = setInterval(() => setTotalSeconds((time) => time + 1), 1000);
    return () => {
      clearInterval(handle);
    };
  }, [info.startTime]);

  const handleConfigSumbit = React.useCallback((event: React.FormEvent) => {
    event.preventDefault();
    const { order, difficulty } = event.target as any;
    console.log(order.value);

    order && dispatch(BoardActions.init(+order.value));
    difficulty && dispatch(GameActions.setDifficulty(+difficulty.value));
    console.log("submitted!");
  }, []);

  const handleGenerateClick = React.useCallback(() => {
    if (loadLock.current) {
      return;
    }
    loadLock.current = true;
    generate(order).then((cells) => dispatch(BoardActions.load(cells)));
  }, [order, loadLock]);

  return (
    <div className="sudoku-game-container">
      <div className="board">
        <Board />
      </div>
      <div className="sidebar">
        <div className="game-info">
          <p>
            难度：
            {difficultyPhraseMap[info.difficulty]}
          </p>

          <p>用时：{totalSeconds} 秒</p>
          <p>步数：{info.totalSteps}</p>
        </div>
        <div className="game-config">
          <form name="config" onSubmit={handleConfigSumbit}>
            <div className="unit">
              <label>阶数：</label>
              <select name="order" defaultValue={order}>
                {[4, 6, 8, 12].map((val) => (
                  <option key={val} value={val}>
                    {val}
                  </option>
                ))}
              </select>
            </div>
            <div className="unit">
              <label>难度：</label>
              <select
                name="difficulty"
                defaultValue={difficultyPhraseMap[info.difficulty]}
              >
                {Object.values(difficultyPhraseMap).map((str, idx) => (
                  <option key={str} value={idx}>
                    {str}
                  </option>
                ))}
              </select>
            </div>
            <input type="submit" value="应用" />
          </form>
        </div>
        <div className="button-group">
          <button onClick={() => {}}>加载</button>
          <button onClick={handleGenerateClick}>生成</button>
          <button onClick={() => {}}>提交</button>
        </div>
      </div>
    </div>
  );
};
