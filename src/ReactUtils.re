let extendBaseStyle = (baseStyle, extendedStyle) =>
  [Some(baseStyle), extendedStyle] |> List.catOptions |> Css.merge;

// At Qwick we tried to follow the "least powerful tool for the job" philosophy and how we interpretted that was to
// default to patterns that gave you the least amount of data you needed in a specific part of the code and to leverage
// more IO.t('a, 'e) instead of things like unit. What we learned was this provided a lot more context into the
// thoughts of the developer when they were writing the code. It also helped code that was more complex really stick
// and let the developer know that this area required a bit more attention than others.
//
// In following that philosophy, we wrapped all of the internal react hooks to both provide less data unless needed,
// to leverage IO to reduce race conditions, and to leverage the type system in ways that also reduced run-time errors

let useMappableState = intitalState => {
  let (state, set) = React.useState(intitalState);
  let set =
    React.useCallback0(newStateFn =>
      IO.suspend(() => set(newStateFn)) |> IO.mapError(ignore)
    );

  (state, set);
};

let useState = getInitialValue => {
  let (value, map) = useMappableState(getInitialValue);

  let set = React.useCallback0(const >> map);

  (value, set);
};

// We wrapped all of the React.string, React.null, etc... in components to provide a more consistent interface
// in the codebase. This also allowed us to do things in the typesystem to avoid more runtime errors (like providing keys
// for elements in an array)
module S = {
  [@react.component]
  let make = (~children) => React.string(children);
};

module F = {
  [@react.component]
  let make = (~children, ~formatter=Float.toString) =>
    <S> {children |> formatter} </S>;
};

module Null = {
  [@react.component]
  let make = () => React.null;
};

module Spread = {
  [@react.component]
  let make = (~props, ~children) => React.cloneElement(children, props);
};
