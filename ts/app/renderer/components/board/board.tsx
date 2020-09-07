import * as React from "react";
import { FC, useCallback, useEffect, useState } from "react";
import { useSudokuSelector, ICell, CellStatus } from "../../models/redux/store";
import { useDispatch } from "react-redux";
import cx from "classnames";
import { BoardActions, BoardActionType } from "../../models/redux/actions";
import "./styles.less";

const generateBoardStyles = (order: number) => ({
  gridTemplateColumns: `repeat(${order}, 1fr)`,
  gridTemplateRows: `repeat(${order}, 1fr)`,
});

interface ICellProps extends ICell {
  onClick?: () => void;
}

const Cell: FC<ICellProps> = ({ mutable, status=true, onClick }) => {
  return (
    <div
      className={cx("sudoku-board-cell", { disable: !mutable })}
      onClick={mutable ? onClick : null}
    >
      <div className="value">
        <p>
          {status !== CellStatus.Unknown
            ? status === CellStatus.True
              ? 1
              : 0
            : ""}
        </p>
      </div>
    </div>
  );
};

export const Board: FC = (props) => {
  const { order, cells } = useSudokuSelector((state) => state.board);
  const dispatch = useDispatch();

  const [boardStyles, setBoardStyles] = useState(generateBoardStyles(order));

  useEffect(() => {
    setBoardStyles(generateBoardStyles(order));
  }, [order]);

  useEffect(() => {
    dispatch(BoardActions.init(6));
  }, []);

  const handleCellClick = useCallback(
    (idx: number) => {
      if (idx >= order ** 2) {
        return;
      }
      if (!cells[idx].mutable) return;
      const newStatus = ((cells[idx].status + 2) % 3) - 1;
      console.log(idx, cells[idx], newStatus);
      dispatch(BoardActions.makeMove(idx, newStatus));
    },
    [order, cells]
  );

  return (
    <div className="sudoku-board-container" style={boardStyles}>
      {cells.map((cell, idx) => (
        <Cell
          mutable={cell.mutable}
          key={idx}
          status={cell.status}
          onClick={() => handleCellClick(idx)}
        />
      ))}
    </div>
  );
};
