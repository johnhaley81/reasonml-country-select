let mergeClassNames = className =>
  List.appendOption(className)
  >> List.intersperse(" ")
  >> List.foldLeft((++), "");

module S = {
  [@react.component]
  let make = (~children) => React.string(children);
};
