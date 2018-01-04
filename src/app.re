[%bs.raw {|require('./app.css')|}];

[@bs.module] external logo : string = "./logo.svg";

let getEquation = () => (Random.int(10) + 1, Random.int(10) + 1);

let getProduct = ((x, y)) => x * y;

let eq = "=";

let notEq = "\226\137\160";

type action =
  | Click
  | KeyDown(int)
  | InputChange(string);

type state = {
  numbers: (int, int),
  guess: int,
  correct: bool
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
  initialState: () => {numbers: getEquation(), guess: 0, correct: false},
  reducer: (action, state) =>
    switch action {
    | Click
    | KeyDown(13) =>
      let product = getProduct(state.numbers);
      let correct = state.guess == product;
      correct ? Js.log("woo") : Js.log("fuck");
      ReasonReact.Update({
        ...state,
        numbers: correct ? getEquation() : state.numbers,
        guess: correct ? 0 : state.guess
      });
    | InputChange(value) =>
      ReasonReact.Update({...state, guess: parseInt(value, state.guess)})
    | KeyDown(_) => ReasonReact.NoUpdate
    },
  render: ({state, reduce}) => {
    let {guess, numbers} = state;
    let x = string_of_int(fst(numbers));
    let y = string_of_int(snd(numbers));
    let equation = {j|$x * $y = $guess|j};
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