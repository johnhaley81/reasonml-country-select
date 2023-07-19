let mergeClassNames = className =>
  List.appendOption(className)
  >> List.intersperse(" ")
  >> List.foldLeft((++), "");

// At Qwick we tried to follow the "least powerful tool for the job" philosophy and how we interpretted that was to
// default to patterns that gave you the least amount of data you needed in a specific part of the code and to leverage
// more IO.t('a, 'e) instead of things like unit. What we learned was this provided a lot more context into the
// thoughts of the developer when they were writing the code. It also helped code that was more complex really stick
// and let the developer know that this area required a bit more attention than others.
//
// In following that philosophy, we wrapped all of the internal react hooks to both provide less data unless needed,
// to leverage IO to reduce race conditions, and to leverage the type system in ways that also reduced run-time errors

let useMappableState = getInitialValue => {
  let (value, setter) = React.useState(getInitialValue);

  let map = React.useCallback0(map => IO.suspend(() => setter(map)));

  (value, map);
};

let useState = getInitialValue => {
  let (value, map) = useMappableState(getInitialValue);

  let set = React.useCallback(const >> map);

  (value, set);
};

// We wrapped all of the React.string, React.null, etc... in components to provide a more consistent interface
// in the codebase. This also allowed us to do things in the typesystem to avoid more runtime errors (like providing keys
// for elements in an array)
module S = {
  [@react.component]
  let make = (~children) => React.string(children);
};

module Null = {
  [@react.component]
  let make = () => React.null;
};
