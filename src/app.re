[%bs.raw {|require('./app.css')|}];

[@bs.module] external logo : string = "./logo.svg";

let getEquation = () => (Random.int(10) + 1, Random.int(10) + 1);

let printEquation = (operands, guess) =>
  switch operands {
  | Some((x, y)) => {j|$x * $y = $guess|j}
  | None => "Done!"
  };

let rec getTimesTable = (table: int, start: int, max: int) =>
  if (start > max) {
    [];
  } else {
    [(table, start), ...getTimesTable(table, start + 1, max)];
  };

let getProduct = ((x, y)) => x * y;

type action =
  | Click
  | KeyDown(int)
  | InputChange(string)
  | SelectTable(int);

type state = {
  guess: int,
  queue: list((int, int)),
  table: int
};

let change = event =>
  InputChange(
    ReactDOMRe.domElementToObj(ReactEventRe.Form.target(event))##value
  );

let keyDown = event => KeyDown(ReactEventRe.Keyboard.which(event));

let parseInt = (value, prev) =>
  switch value {
  | "" => 0
  | _ =>
    try (int_of_string(value)) {
    | Failure("int_of_string") => prev
    }
  };

let getOperands = queue =>
  switch queue {
  | [] => None
  | [hd, ..._] => Some(hd)
  };

let isCorrect = (operands, guess) => getProduct(operands) === guess;

let component = ReasonReact.reducerComponent("App");

let initialTable = 3;

let make = _children => {
  ...component,
  initialState: () => {
    guess: 0,
    queue: getTimesTable(initialTable, 1, 10),
    table: initialTable
  },
  reducer: (action, state) =>
    switch action {
    | Click
    | KeyDown(13) =>
      switch state.queue {
      | [hd, ...tl] =>
        let isCorrect = getProduct(hd) === state.guess;
        ReasonReact.Update({
          ...state,
          queue: isCorrect ? tl : state.queue,
          guess: 0
        });
      | [] => ReasonReact.Update({...state, guess: 0})
      }
    | InputChange(value) =>
      ReasonReact.Update({...state, guess: parseInt(value, state.guess)})
    | KeyDown(_) => ReasonReact.NoUpdate
    | SelectTable(table) =>
      ReasonReact.Update({...state, queue: getTimesTable(table, 1, 10), table})
    },
  render: ({state, reduce}) => {
    let {guess, queue, table} = state;
    let operands = getOperands(queue);
    let equation = printEquation(operands, guess);
    <div className="App">
      <div className="App-header">
        <img src=logo className="App-logo" alt="logo" />
        <TablePicker onSelect=(reduce(i => SelectTable(i))) active=table />
      </div>
      <p className="App-intro"> (ReasonReact.stringToElement(equation)) </p>
      <input
        onChange=(reduce(change))
        onKeyDown=(reduce(keyDown))
        value=(string_of_int(guess))
      />
      <button onClick=(reduce(_event => Click))>
        (ReasonReact.stringToElement("Guess"))
      </button>
    </div>;
  }
};