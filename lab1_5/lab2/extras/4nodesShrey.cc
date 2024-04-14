/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
// Default Network Topology
//
//       10.1.1.0
// n0 -------------- n1
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
  nodes.Create (4);
  PointToPointHelper p21;
  p21.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  p21.SetChannelAttribute ("Delay", StringValue ("2ms"));
 
  PointToPointHelper p32;
  p32.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  p32.SetChannelAttribute ("Delay", StringValue ("5ms"));
 
  PointToPointHelper p43;
  p43.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  p43.SetChannelAttribute ("Delay", StringValue ("5ms"));
  NetDeviceContainer devices;
  NetDeviceContainer n1 = p21.Install (nodes.Get(0),nodes.Get(1));
  NetDeviceContainer n2 = p32.Install (nodes.Get(1),nodes.Get(2));
  NetDeviceContainer n3 = p43.Install (nodes.Get(2),nodes.Get(3));
  InternetStackHelper stack;
  stack.Install (nodes);
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i1 = address.Assign (n1);
  Ipv4AddressHelper address1;
  address.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer i2 = address.Assign (n2);
  Ipv4AddressHelper address2;
  address.SetBase ("10.1.3.0", "255.255.255.0");
  Ipv4InterfaceContainer i3 = address.Assign (n3);
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
  //Ipv4InterfaceContainer interfaces = address.Assign (devices);
  UdpEchoServerHelper echoServer (9);
  ApplicationContainer serverApps = echoServer.Install (nodes.Get(3));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));
  UdpEchoClientHelper echoClient (i3.GetAddress (1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));
  ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
