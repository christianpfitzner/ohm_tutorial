/* turtle_control_node.cpp */



#include "ros/ros.h"                    // allgemeiner Header für ROS
#include <geometry_msgs/Twist.h>        // Nachricht zum Verschicken von Geschwindigkeitsbefehlen
#include <turtlesim/Spawn.h>            // Spawn Service
#include <turtlesim/Pose.h>             // Nachricht für die Pose der Schildkröte
#include <turtlesim/Kill.h>
#include <turtlesim/SetPen.h>

ros::ServiceClient pen_client; 
ros::Publisher vel_pub; 
ros::Subscriber my_pose_sub;
ros::Subscriber target_pose_sub;

turtlesim::Pose target_pose;
turtlesim::Pose my_pose;


void callbackMyPose(const turtlesim::Pose& pose)
{
   my_pose = pose;
}

void callbackTargetPose(const turtlesim::Pose& pose)
{
   target_pose = pose;
}


int main(int argc, char **argv)
{
  if(argc<2)
  {
    std::cout << "Too less parameters. Expected: " << argv[0] << " <turtlename>" << std::endl;
    return -1;
  }


  /**
   * Name des eigenen Knotens
   */
  char node[64];
  sprintf(node, "%s%s", argv[1], "_node");
  printf("Neuer Knoten: %s\n", node);

  ros::init(argc, argv, node);
  ros::NodeHandle n;


  /**
   * Name des publishers
   */
  char pubname[64];
  sprintf(pubname, "%s/%s", argv[1], "cmd_vel");
  printf("Publisher: %s\n", pubname);


  char penname[64]; 
  sprintf(penname, "%s/%s", argv[1], "set_pen"); 
  /**
   * Implementieren Sie hier den Aufruf des Dienstes spawn
   */
   ros::ServiceClient client      = n.serviceClient<turtlesim::Spawn>("spawn"); 
/*
    * Senden Sie einen Service-Call über den Service client ab
    * Beachten Sie, dass der name der Schildkröte mit argv[1] gesetzt wird, so dass
    * jede Schildkröte im Netzwerk nur einmal vorkommt.
    */
   turtlesim::Spawn srv;
   srv.request.x = 5.0; 
   srv.request.y = 5.0;
   srv.request.name = argv[1];   
   client.call(srv); 


   /* Für Schnelle:
    * 1) Ändern Sie die Farbe der Schildkröte in abhängigkeit ihrer x, y Position.
    *    Seien Sie kreativ!
    * 2) Lassen Sie die Schildkröte ein Rechteck abfahren.
    *
    */
  char posename[64]; 
  sprintf(posename, "%s/%s", argv[1], "pose"); 
  my_pose_sub     = n.subscribe(posename, 1, callbackMyPose);
  target_pose_sub = n.subscribe("chris/pose", 1, callbackTargetPose);

  /**
   * Implementieren Sie hier das Versenden von Nachrichten
   * zur Steuerung der Schildkröte
   */
   vel_pub = n.advertise<geometry_msgs::Twist>(pubname, 1); 

   ros::Rate r(10); 

   unsigned int i=0; 

   while(ros::ok())
   {
      // calculate linear velocity based on euclidean distance


      // calculate angle between your own turlte and the target

      geometry_msgs::Twist msg; 
      msg.linear.x  = 4.0; 
      msg.angular.z = 1.0; 




      vel_pub.publish(msg); 

      ros::spinOnce();  
      r.sleep(); 

   }


  return 0;
}


