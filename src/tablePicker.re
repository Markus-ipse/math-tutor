let component = ReasonReact.statelessComponent("TablePicker");

let tables = Array.init(9, i => i + 1);

let make = (~onSelect, ~active, _children) => {
  ...component,
  render: self => {
    let kids =
      Array.map(
        i =>
          <button
            key=(string_of_int(i))
            onClick=(_evt => onSelect(i))
            className=(active === i ? "is-active" : "")>
            (ReasonReact.stringToElement(string_of_int(i)))
          </button>,
        tables
      );
    ReasonReact.arrayToElement(kids);
  }
};