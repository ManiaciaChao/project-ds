import * as React from "react";
import { Link } from "react-router-dom";
import { Game } from "./game/game";
import { Provider } from "react-redux";
import { sudokuStore } from "../models/redux/store";

const styles: React.CSSProperties = {
  textAlign: "center",
};

export default () => (
  <div style={styles}>
    <h3>
      <Link to="/">Back</Link>
    </h3>
    <Provider store={sudokuStore}>
      <Game />
    </Provider>
  </div>
);
