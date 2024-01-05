import MainComponent from "./file";
import React, { FC } from "react";
interface Return {
  s: string;
  n: number;
}
export const func = (r: Return): Return => {
  return { s: "hey", n: 10 };
};
type FunctionReturn<T> = (n: T) => any;
const rf = (f: FunctionReturn<string>): void => {};
const funct = (): void => {};
rf(funct);
const return_val: Return = func({ n: 10, s: "jey" });
const { n, s } = func({ n: 10, s: "s" });
const C: FC = () => {
  const { InnerComp, num } = MainComponent(1);
  return (
    <>
      <div>
        <InnerComp data={1} />
      </div>
    </>
  );
};
export default C;
