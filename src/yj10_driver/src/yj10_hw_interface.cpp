#include "yj10_driver/yj10_hw_interface.h"

Yj10HWInterface::Yj10HWInterface(ros::NodeHandle &nh, urdf::Model *urdf_model)
    : ros_control_boilerplate::GenericHWInterface(nh, urdf_model)
{
    ROS_INFO_NAMED("yj10_hw_interface", "Yj10HWInterface Ready.");
}

void Yj10HWInterface::read(ros::Duration &elapsed_time)
{
    if (is_fake_connect)
    {
        return;
    }

    if (is_connected)
    {
        // 如果失败，就多尝试几次
        for (size_t i = 0; i < read_retry_time; i++)
        {
            try
            {
                arm.ReadAllJointsPwm();
                for (size_t i = 0; i < num_joints_; i++)
                {
                    joint_position_.at(i) = arm.JointRad(i);
                }
            }
            catch (const std::exception &e)
            {
                ROS_ERROR_STREAM("Read failed. what(): " << e.what());
            }
        }
    }
}

void Yj10HWInterface::write(ros::Duration &elapsed_time)
{
    // Safety
    enforceLimits(elapsed_time);

    if (is_fake_connect)
    {
        for (size_t i = 0; i < num_joints_; i++)
        {
            joint_position_.at(i) = joint_position_command_.at(i);
        }
        return;
    }

    if (is_connected)
    {
        std::array<double, 5> rads;

        for (size_t i = 0; i < rads.size(); i++)
        {
            rads.at(i) = joint_position_command_.at(i);
        }

        // 如果写入失败，就多尝试几次
        for (size_t i = 0; i < write_retry_time; i++)
        {
            try
            {
                arm.WriteAllJointsRad(rads);
                break;
            }
            catch (const std::exception &e)
            {
                ROS_ERROR_STREAM("Write failed. what(): " << e.what());
            }
        }
    }
}

void Yj10HWInterface::enforceLimits(ros::Duration &period)
{
    // ----------------------------------------------------
    // ----------------------------------------------------
    // ----------------------------------------------------
    //
    // CHOOSE THE TYPE OF JOINT LIMITS INTERFACE YOU WANT TO USE
    // YOU SHOULD ONLY NEED TO USE ONE SATURATION INTERFACE,
    // DEPENDING ON YOUR CONTROL METHOD
    //
    // EXAMPLES:
    //
    // Saturation Limits ---------------------------
    //
    // Enforces position and velocity
    pos_jnt_sat_interface_.enforceLimits(period);
    //
    // Enforces velocity and acceleration limits
    // vel_jnt_sat_interface_.enforceLimits(period);
    //
    // Enforces position, velocity, and effort
    // eff_jnt_sat_interface_.enforceLimits(period);

    // Soft limits ---------------------------------
    //
    // pos_jnt_soft_limits_.enforceLimits(period);
    // vel_jnt_soft_limits_.enforceLimits(period);
    // eff_jnt_soft_limits_.enforceLimits(period);
    //
    // ----------------------------------------------------
    // ----------------------------------------------------
    // ----------------------------------------------------
}