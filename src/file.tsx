import React, { FC } from "react";
interface Props {
  data: number;
}
const MainComponent = (i: number) => {
  let num: number = i;
  const InnerComp: FC<Props> = ({ data }) => {
    return (
      <>
        <div>{data}</div>
      </>
    );
  };
  return { InnerComp, num };
};
export default MainComponent;
