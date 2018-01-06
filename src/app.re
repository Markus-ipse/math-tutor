[%bs.raw {|require('./app.css')|}];

[@bs.module] external logo : string = "./logo.svg";

let getEquation = () => (Random.int(10) + 1, Random.int(10) + 1);

let printEquation = (numbers, guess) =>
  switch numbers {
  | Some((x, y)) => {j|$x * $y = $guess|j}
  | None => "Done!"
  };

let rec getList = (table: int, start: int, max: int) =>
  if (start > max) {
    [];
  } else {
    [(table, start), ...getList(table, start + 1, max)];
  };

let getProduct = ((x, y)) => x * y;

type action =
  | Click
  | KeyDown(int)
  | InputChange(string);

type state = {
  numbers: option((int, int)),
  guess: int,
  correct: bool,
  queue: list((int, int))
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

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  initialState: () => {
    let [head, ...tail] = getList(3, 1, 10);
    {numbers: Some(head), guess: 0, correct: false, queue: tail};
  },
  reducer: (action, state) =>
    switch action {
    | Click
    | KeyDown(13) =>
      let product =
        switch state.numbers {
        | Some(nums) => getProduct(nums)
        | None => 9999
        };
      let correct = state.guess == product;
      let nextNumbers =
        switch state.queue {
        | [] => None
        | [hd, ..._] => Some(hd)
        };
      let updatedQueue =
        switch state.queue {
        | [] => []
        | [_, ...tl] => tl
        };
      if (correct) {
        Js.log(printEquation(state.numbers, state.guess));
      };
      ReasonReact.Update({
        ...state,
        numbers: correct ? nextNumbers : state.numbers,
        queue: correct ? updatedQueue : state.queue,
        guess: correct ? 0 : state.guess
      });
    | InputChange(value) =>
      ReasonReact.Update({...state, guess: parseInt(value, state.guess)})
    | KeyDown(_) => ReasonReact.NoUpdate
    },
  render: ({state, reduce}) => {
    let {guess, numbers} = state;
    let equation = printEquation(numbers, guess);
    <div className="App">
      <div className="App-header">
        <img src=logo className="App-logo" alt="logo" />
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