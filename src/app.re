[%bs.raw {|require('./app.css')|}];

[@bs.module] external logo : string = "./logo.svg";

let getEquation = () => (Random.int(10), Random.int(10));

type action =
  | Click;

type state = {
  numbers: (int, int),
  guess: int
};

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  initialState: () => {numbers: getEquation(), guess: 0},
  reducer: (action, state) =>
    switch action {
    | Click => ReasonReact.Update({guess: 1, numbers: getEquation()})
    },
  render: self => {
    let (x, y): (int, int) = self.state;
    let equation = string_of_int(x) ++ " * ";
    <div className="App">
      <div className="App-header">
        <img src=logo className="App-logo" alt="logo" />
      </div>
      <p className="App-intro"> (ReasonReact.stringToElement(equation)) </p>
      <button onClick=(self.reduce(_event => Click)) />
    </div>;
  }
};