#include <ros/ros.h>

#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>
#include <std_msgs/String.h>

void pubLineStrip(const ros::Publisher& pub) {
  // clear the exist marker/markers in rviz with same topics of publisher
  // DELETEALL will ignore ns and id. (TODO: should be tested)
  visualization_msgs::Marker clear_marker;
  clear_marker.action = visualization_msgs::Marker::DELETEALL;
  pub.publish(clear_marker);

  visualization_msgs::Marker line_strip_marker;
  line_strip_marker.header.frame_id = "base_link";
  line_strip_marker.type = visualization_msgs::Marker::LINE_STRIP;
  line_strip_marker.action = visualization_msgs::Marker::ADD;

  line_strip_marker.scale.x = 0.7;
  // set whole color of line strip
  // line_strip_marker.color.a = 0.5;
  // line_strip_marker.color.r = 0.5;
  // line_strip_marker.color.g = 0.5;
  // line_strip_marker.color.b = 0.5;

  constexpr double kPointSize = 101;
  for (int i = 0; i < kPointSize; ++i) {
    geometry_msgs::Point point;
    constexpr double kResolution = 0.5;
    point.x = i * kResolution;
    point.y = 0.0;
    line_strip_marker.points.push_back(point);

    // set color for every point according to some rules, such as speed
    constexpr double kSpeedResolution = 0.2;
    double v = i * kSpeedResolution;
    std_msgs::ColorRGBA color;
    color.a = 1.0;
    color.r = 0.0 + (1.0 - 0.0) * v / (kSpeedResolution * kPointSize - 0.0);
    color.g = 0.0;
    color.b = 0.0;
    line_strip_marker.colors.push_back(color);
  }

  pub.publish(line_strip_marker);
}

void pubLineStripBox(const ros::Publisher& pub) {
  visualization_msgs::Marker clear_marker;
  clear_marker.action = visualization_msgs::Marker::DELETEALL;
  pub.publish(clear_marker);

  visualization_msgs::Marker box_marker;
  box_marker.header.frame_id = "base_link";
  box_marker.ns = "strip_box";
  box_marker.type = visualization_msgs::Marker::LINE_STRIP;
  box_marker.action = visualization_msgs::Marker::ADD;
  box_marker.scale.x = 0.2;
  box_marker.color.a = 0.8;
  box_marker.color.r = 1.0;
  box_marker.color.g = 1.0;
  box_marker.color.b = 1.0;

  geometry_msgs::Point point;
  point.x = -1.0; point.y = -0.5;
  box_marker.points.push_back(point);
  point.x = 1.0;
  box_marker.points.push_back(point);
  point.y = 0.5;
  box_marker.points.push_back(point);
  point.x = -1.0;
  box_marker.points.push_back(point);
  point.y = -0.5;
  box_marker.points.push_back(point);

  visualization_msgs::MarkerArray markers;
  markers.markers.push_back(box_marker);
  pub.publish(markers);
}

int main(int argc, char** argv) {
  ros::init(argc, argv, "rviz_example_node");
  ros::NodeHandle nh;

  ros::Publisher line_strip_pub = nh.advertise<visualization_msgs::Marker>("line_strip_viz", 1);
  ros::Publisher line_strip_box_pub =
      nh.advertise<visualization_msgs::MarkerArray>("line_strip_box_viz", 1);

  ros::Rate r(1);
  while (ros::ok()) {
    ros::spinOnce();
    pubLineStrip(line_strip_pub);
    pubLineStripBox(line_strip_box_pub);
    r.sleep();
  }

  return 0;
}
