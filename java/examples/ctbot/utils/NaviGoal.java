package ctbot.utils;

public class NaviGoal {
  public NaviGoal nextGoal = null;
  public NaviGoal subGoal = null;
  
  protected boolean work() {
    return false;
  }

  public final boolean doWork() {
    if (subGoal==null)
      return work();
    if (!subGoal.doWork()){
      this.subGoal=subGoal.nextGoal;
    }
    return true;
  }

  public void setNextGoal(NaviGoal goal){
    this.nextGoal=goal;
  }
}
