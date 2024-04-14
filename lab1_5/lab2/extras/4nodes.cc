#include "ns3/netanim-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

// Default Network Topology
//
//       10.1.1.0       10.1.2.0      10.1.3.0
// n0 -------------- n1------------n2------------n3
//    point-to-point
//
 
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

int
main (int argc, char *argv[])
{
  CommandLine cmd (__FILE__);
  cmd.Parse (argc, argv);
  
  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  NodeContainer nodes;
  nodes.Create (4);//nodes4
  
  InternetStackHelper stack;
  stack.Install (nodes);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
 //three p2p
  
  NetDeviceContainer devices;
  devices = pointToPoint.Install (nodes.Get(0),nodes.Get(1));
  
  NetDeviceContainer devices1;
  devices1 = pointToPoint.Install (nodes.Get(1),nodes.Get(2));
  
   NetDeviceContainer devices2;
  devices2 = pointToPoint.Install (nodes.Get(2),nodes.Get(3));

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4AddressHelper address1;
  address1.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4AddressHelper address2;
  address2.SetBase ("10.1.3.0", "255.255.255.0");
  

  Ipv4InterfaceContainer interfaces = address.Assign (devices);
  Ipv4InterfaceContainer interfaces1 = address1.Assign (devices1);
  Ipv4InterfaceContainer interfaces2 = address2.Assign (devices2);
  
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  UdpEchoServerHelper echoServer  (90);
  //UdpEchoServerHelper echoServer1 (91);
  //UdpEchoServerHelper echoServer2 (92);

  ApplicationContainer serverApps = echoServer.Install (nodes.Get (3));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (interfaces2.GetAddress (1) ,90);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));
  
  /*UdpEchoClientHelper echoClient1 (interfaces.GetAddress (1), 91);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));
  
  UdpEchoClientHelper echoClient2 (interfaces.GetAddress (1), 92);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));*/

  ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));
 /* ApplicationContainer clientApps1 = echoClient.Install (nodes.Get (2));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));
  ApplicationContainer clientApps2 = echoClient.Install (nodes.Get (3));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));*/
  
  
  AnimationInterface anim ("anim1.xml");
  anim.SetConstantPosition(nodes.get(0),1.0,2.0);
  anim.SetConstantPosition(nodes.get(1),2.0,2.0); 
  anim.SetConstantPosition(nodes.get(2),3.0,2.0);
  
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}




//or https://www.youtube.com/watch?v=6gCVgWNPA3s
