#include "pch.hpp"
#include "input_routes.hpp"
#include "scaleform/router.hpp"
#include "scaleform/route_helpers.hpp"
#include "scaleform/util.hpp"
#include "orientation_markers_routes.h"

#include "entities_hud.hpp"
#include "script_player.hpp"

#include "scaleform/sf_movie_manager.hpp"

BW_BEGIN_NAMESPACE

using namespace SF;

namespace Hammer
{

#ifdef ROUTES_DEBUG
#pragma optimize("", off)
#endif

void split (const std::string& s, char delim, std::vector<std::string>& elems) 
{
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) 
    {
        elems.push_back(item);
    }
}


std::vector<std::string> split (const std::string& s, char delim) 
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

PyObject* findObject (PyObject* root, const std::string& fullname, const std::vector <std::string>& path)
{
    PyObject* obj = root;
    for (auto& name: path)
    {
        if (PyInstance_Check(obj))
        {
    		PyInstanceObject* inst = (PyInstanceObject*) obj;
			PyObject* key = NULL;
			PyObject* val = NULL;
			Py_ssize_t pos = 0;
            obj = nullptr;
    		while (PyDict_Next(inst->in_dict, &pos, &key, &val))
			{
				std::string argName = PyString_AsString(key);
                if (argName == name) 
                {
                    obj = val;
                    break;
                }
			}
        } 
        else
        {
			obj = PyObject_GetAttrString(obj, name.c_str());
        }
        if (obj == nullptr) 
        {
            TRACE_MSG("[PythonWStrInput]: wrong python attribute: %s, field %s\n", fullname.c_str(), name.c_str());
        }
    }
    return obj;
}

bool world_to_screen (const Matrix& m, const Vector3& input, Vector2& pos)
{
	Vector4	v;
	m.applyPoint (v, input);
	v.w = 1.f / v.w;
	v.z *= v.w;
	v.x = (v.x * v.w * Moo::rc().halfScreenWidth()) + Moo::rc().halfScreenWidth();
	v.y = (-v.y * v.w * Moo::rc().halfScreenHeight()) + Moo::rc().halfScreenHeight();
    pos.x = v.x;
    pos.y = v.y;
	if((v.w >= 0) && (v.x > 0) && (v.y > 0) && (v.x < Moo::rc().screenWidth()) && (v.y < Moo::rc().screenHeight()))
    {
        return true;    
    }
    return false;    
}

bool world_to_screen (const Vector3& input, Vector2& pos)
{
	Matrix	objectToClip;
	objectToClip.multiply (Moo::rc().world(), Moo::rc().viewProjection());
    return world_to_screen (objectToClip, input, pos);
}

bool calculate_entity_position (const EntityIDList& entity_ids, Vector3& pos)
{
    pos = Vector3::zero ();
    int count = 0;
    for (const auto entity_id: entity_ids)
    {
        Entity* entity = EntityManager::instance().getEntity (entity_id, true);
        if (entity != NULL)
        {
            pos += entity->position ();
            count++;
        }
    }
    if (count > 0)
    {
        pos /= static_cast <float> (count);
    }
    return count > 0;
}

Vector3 calculate_entity_angles (Entity* entity)
{
	Matrix invView = Moo::rc().invView();
	Vector3 cameraPos (invView._41, invView._42, invView._43);
	Vector3 cameraDir (invView._31, invView._32, invView._33);
	Vector3 cameraLeft (invView._21, invView._22, invView._23);
	Vector3 cameraUp (invView._11, invView._12, invView._13);

	Vector3 entityDir (entity->position() - cameraPos);
	cameraDir.normalise();
	entityDir.normalise();
    Vector3 angles;
	angles.z = cameraDir.dotProduct (entityDir);
    angles.x = asin (cameraUp.dotProduct (entityDir));
    angles.y = asin (cameraLeft.dotProduct (entityDir));
    return angles;
}

Vector2 calculate_offscreen_position (Entity* entity, const Vector2& shift)
{
    Vector2 out;
    Vector3 angles = calculate_entity_angles (entity);
    float y_fov = Moo::rc().camera ().fov () / 2;
    float x_fov = y_fov * Moo::rc().camera ().aspectRatio();

    float half_width = Moo::rc().halfScreenWidth() - shift.x;
    float half_height = Moo::rc().halfScreenHeight() - shift.y;

    float x = 0.0f;
    float y = 0.0f;
    if (angles.x < - x_fov)
    {
        x = - half_width;
    } 
    else if (angles.x > x_fov)
    {
        x = half_width;
    }
    else
    {
        if (angles.z > 0)
        {
            x = Moo::rc().halfScreenWidth() * abs (angles.x) / x_fov;
            if (x > half_width) x = half_width;
            x *= (angles.x > 0 ? 1.0f : -1.0f);
        }
        else 
        {
            x = (angles.x > 0 ? 1.0f : -1.0f) * half_width;
        }
    }


    if (angles.y < - y_fov)
    {
        y = - half_height;
    } 
    else if (angles.y > y_fov)
    {
        y = half_height;
    }
    else
    {
        y = Moo::rc().halfScreenHeight() * abs (angles.y) / y_fov;
        if (y > half_height) y = half_height;
        y *= (angles.y > 0 ? 1.0f : -1.0f);
    }

    out.x = Moo::rc().halfScreenWidth() + x;
    out.y = Moo::rc().halfScreenHeight() - y;
    return out;
}

float calculate_offscreen_direction (Entity* entity)
{
    Vector2 pos = calculate_offscreen_position (entity, Vector2 (0, 0));
    Vector2 center (Moo::rc().halfScreenWidth(), Moo::rc().halfScreenHeight());
    Vector2 delta = pos - center;
    delta.normalise();
    float angle = atan2 (delta.y, delta.x) + MATH_PI;
    return angle;
}

bool calculate_2d_bbox (EntityID entity_id, Vector4& out)
{
    Vector2 align (0.0f, 0.0f);
    Entity* entity = EntityManager::instance().getEntity (entity_id, true);
    if (entity == NULL) return false;
    Vector3 pos, posScreen, dim;
    bool visible = entity->getScreenPosition (posScreen, dim, align);
//	if (visible) return false;

	const Direction3D &direction = entity->direction();
	Matrix m;
	m.setRotate(direction.yaw, direction.pitch, direction.roll);
	m.translation(entity->position());
	m.postMultiply(Moo::rc().viewProjection());

	size_t partCount = entity->getBBoxInfo().partCount();
    if (partCount == 0) return false;

	Vector3 min_point (FLT_MAX, FLT_MAX, FLT_MAX);
	Vector3 max_point (-FLT_MAX, -FLT_MAX, -FLT_MAX);

   	Vector2	point_2d;
    if (!world_to_screen (m, entity->getBBoxInfo().totalAABB().centre (), point_2d)) return false;

    Vector2 min_point_2d (FLT_MAX, FLT_MAX);
    Vector2 max_point_2d (-FLT_MAX, -FLT_MAX);
    bool ret = false;
    for (size_t ip = 0; ip < partCount; ++ip)
	{
		const BoundingBox &bb = entity->getBBoxInfo().partAABB(ip);
		Vector3 bounds[2] = {bb.minBounds(), bb.maxBounds()};
		for (uint i = 0; i < 8; ++i)
		{
			Vector3 p (bounds[i & 1].x, bounds[i >> 1 & 1].y, bounds[i >> 2 & 1].z); //get all 8 corner of BBOX
            world_to_screen (m, p, point_2d);
            min_point_2d.x = min (min_point_2d.x, point_2d.x);
            min_point_2d.y = min (min_point_2d.y, point_2d.y);
            max_point_2d.x = max (max_point_2d.x, point_2d.x);
            max_point_2d.y = max (max_point_2d.y, point_2d.y);
            ret = true;
		}
	}

    if (ret) 
    {
        out.x = min_point_2d.x;
        out.y = min_point_2d.y;
        out.z = max_point_2d.x;
        out.w = max_point_2d.y;
    }
    return ret;
}

/////////////////////////

class PositionInput: public Router::IInput<Vector2>
{
    Vector2  align_;
    Vector2  shift;
    EntityID entityID_;
    bool spherical;
    float spherical_scale;
    PyInputBind py_input_bind;
public:
    bool read(Vector2& out)
    {
        Entity* entity = EntityManager::instance().getEntity(entityID_, true);
        if (entity == NULL)
        {
            return false;
        }

        out.x = -500.f;
        out.y = -500.f;
        if (py_input_bind.group.size () > 1)
        {
            Vector3 pos;
            if (calculate_entity_position (py_input_bind.group, pos))
            {
                if (world_to_screen (pos, out))
                {
                    float width = Moo::rc().screenWidth() - shift.x;
                    float height = Moo::rc().screenHeight() - shift.y;
                    out.x = max (shift.x, min (width, out.x));
                    out.y = max (shift.y, min (height, out.y));
                    return true;
                }
            }
        }
        else
        {
            Vector3 pos, dim;
            if (entity->getBBoxInfo().partCount()) 
            {
                Vector3 pos, dim;
                bool visible = false;
                if (spherical)
                {
                    visible = entity->getScreenPosition (pos, dim, Vector2 (0.0f, 0.0f));
                    Vector2 dim2 (dim.x / 2, dim.y / 2);
                    pos.y += dim2.length () * spherical_scale;
                }
                else
                {
                    visible = entity->getScreenPosition (pos, dim, align_);
                }
                if (visible) 
                {
                    out.x = pos.x;
                    out.y = pos.y;
                    //float width = Moo::rc().screenWidth() - shift.x;
                    //float height = Moo::rc().screenHeight() - shift.y;
                    //out.x = max (shift.x, min (width, out.x));
                    //out.y = max (shift.y, min (height, out.y));
                }
            } 
            else
            {
                out = calculate_offscreen_position (entity, shift);
            }
        }

        return true;
    }
    PositionInput (const PyInputBind& input_bind): py_input_bind (input_bind), entityID_(input_bind.entity_id), align_ (0.0f, 1.0f), shift (0.0f, 0.0f), spherical (false), spherical_scale (1.0f)
    {
        get_float_value (py_input_bind.config_object, "spherical_scale", spherical_scale);
        get_bool_value (py_input_bind.config_object, "spherical", spherical);

        get_float_value (py_input_bind.config_object, "shift_x", shift.x);
        get_float_value (py_input_bind.config_object, "shift_y", shift.y);
        get_float_value (py_input_bind.config_object, "align_x", align_.x);
        get_float_value (py_input_bind.config_object, "align_y", align_.y);
    }

};

/////////////////////////

class PartPositionInput: public Router::IInput<Vector2>
{
    Vector2     align_;
    EntityID entityID_;
    int part_id;
    
public:
    bool read(Vector2& out)
    {
        Entity* entity = EntityManager::instance().getEntity(entityID_, true);
        if (entity == NULL)
        {
//            TRACE_MSG("[PositionInput]: wrong entity id: %d\n", entityID_);
            return false;
        }

        Vector3 pos, posScreen, dim;
        bool visible = entity->getScreenPosition (posScreen, dim, align_);
	    if (visible)
	    {
		    size_t indexPartInBB = entity->getPartBBIndexByPartID(part_id);
		    if (indexPartInBB < 0)
		    {
			    return false;
		    }

		    const BoundingBox &bb = entity->getBBoxInfo().partAABB(indexPartInBB);
			const Direction3D &direction = entity->direction();
			Matrix m;
			m.setRotate(direction.yaw, direction.pitch, direction.roll);
			m.translation(entity->position());
			m.postMultiply(Moo::rc().viewProjection());
		    Vector3 c = bb.centre();
		    Entity::worldToScreen (pos, c, m);

        	if ((pos.z > 0) && (pos.z < 1) && (pos.x > -1) && (pos.x < 1) && (pos.y > -1) && (pos.y < 1)) 
            {
		        pos.x = (pos.x + 1.0f) * Moo::rc().halfScreenWidth();
		        pos.y = (1.0f - pos.y) * Moo::rc().halfScreenHeight();

                out.x = pos.x;
                out.y = pos.y;
    		    return true;
            }
	    }
        out.x = -500.f;
        out.y = -500.f;

        return true;
    }
    PartPositionInput (const PyInputBind& py_input_bind): entityID_(py_input_bind.entity_id), align_ (0.0f, 0.0f)
    {
        get_int_value (py_input_bind.config_object, "part_id", part_id);
    }
};

class EntityPartsBBoxInput: public Router::IInput<Vector4>
{
    Vector2     align_;
    PyInputBind py_input_bind;
    using PartIDList = std::vector <int>;
    PartIDList part_ids;
public:
    bool read (Vector4& out)
    {
        out.x = -500.f;
        out.y = -500.f;
        out.z = -500.f;
        out.w = -500.f;
        Entity* entity = EntityManager::instance().getEntity (py_input_bind.entity_id, true);
        if (entity == NULL)
        {
            return false;
        }
        if (calculate_2d_bbox (py_input_bind.entity_id, out))
        {
#ifdef ROUTES_DEBUG
            //DebugDrawApp::instance.Add2DRect("Routing", Vector2(out.x, out.y), Vector2(out.z, out.w), 0xFF0000FF);
#endif
        }
        return true;
    }
    EntityPartsBBoxInput (const PyInputBind& input_bind): py_input_bind (input_bind), align_ (0.0f, 0.0f)
    {
        get_int_array_value (py_input_bind.config_object, "part_ids", part_ids);
    }
};

///////////////////////////////////////////////////

class GroupPositionInput: public Router::IInput<Vector2>
{
    EntityID entityID_;
public:
    bool read(Vector2& out)
    {
        Entity* entity = EntityManager::instance().getEntity(entityID_, true);
        if (entity == NULL)
        {
//            TRACE_MSG("[PositionInput]: wrong entity id: %d\n", entityID_);
            return false;
        }
        return false;
    }

    GroupPositionInput (const PyInputBind& py_input_bind): entityID_(py_input_bind.entity_id) 
    {
    }
};

///////////////////////////////////////////////////


class MiniMapPositionInput: public Router::IInput<Vector2>
{
    Vector2 position;
    Vector2 map_size;
    Vector2 world_size;
    Vector2 world_min;
    Vector2 world_max;
    Vector2 scale;
    EntityID entityID_;
    PyInputBind py_input_bind;
public:
    bool read(Vector2& out)
    {

        Vector3 pos;
        if (!calculate_entity_position (py_input_bind.group, pos))
        {
            return false;
        }

        out.x = position.x + (pos.x - world_min.x) * scale.x;
        out.y = position.y + (pos.z - world_min.y) * scale.y;

        return true;
    }
    MiniMapPositionInput (const PyInputBind& input_bind): py_input_bind (input_bind), entityID_(input_bind.entity_id), position (0.0f, 0.0f), world_size (1000.f, 1000.f) 
    {
        map_size = Vector2 (Moo::rc().screenWidth (), Moo::rc().screenHeight ());
        if (!py_input_bind.config_object.IsNull ())
        {
            get_float_value (py_input_bind.config_object, "position_x", position.x);
            get_float_value (py_input_bind.config_object, "position_y", position.y);

            get_float_value (py_input_bind.config_object, "map_size_x", map_size.x);
            get_float_value (py_input_bind.config_object, "map_size_y", map_size.y);

            get_float_value (py_input_bind.config_object, "world_min_x", world_min.x);
            get_float_value (py_input_bind.config_object, "world_min_y", world_min.y);
            get_float_value (py_input_bind.config_object, "world_max_x", world_max.x);
            get_float_value (py_input_bind.config_object, "world_max_y", world_max.y);

            world_size = world_max - world_min;

            scale.x = map_size.x / world_size.x;
            scale.y = map_size.y / world_size.y;
        }
    }
};

///////////////////////////////////////////////////

class MiniMapRotationInput: public Router::IInput<float>
{
//    Vector2 nord_dir;
    EntityID entityID_;
public:
    bool read (float& out)
    {
        Entity* entity = EntityManager::instance().getEntity(entityID_, true);
        if (entity == NULL)
        {
//            TRACE_MSG("[PositionInput]: wrong entity id: %d\n", entityID_);
            return false;
        }

        out = entity->direction ().yaw;

        return true;
    }
    MiniMapRotationInput (const PyInputBind& py_input_bind): entityID_(py_input_bind.entity_id) {} 
};

///////////////////////////////////////////////////

class MiniMapCameraRotationInput: public Router::IInput<float>
{
    EntityID entityID_;
public:
    bool read (float& out)
    {
	    Matrix invView = Moo::rc().invView();
        Vector2 camera_direction (invView._31, invView._33);
        out = atan2 (camera_direction.x, camera_direction.y);
        return true;
    }
    MiniMapCameraRotationInput (const PyInputBind& py_input_bind): entityID_(py_input_bind.entity_id) {} 
};

///////////////////////////////////////////////////

float get_radar_distance (const Vector2& target_pos, Vector2& delta, float &angle)
{
	Matrix invView = Moo::rc().invView();
    Vector2 camera_pos (invView._41, invView._43);
    delta = target_pos - camera_pos;
    angle = atan2 (delta.y, delta.x);
    return delta.length ();
}

class RadarPositionInput: public Router::IInput<Vector2>
{
    RadarMinimapContext context;
    EntityID entityID_;
    PyInputBind py_input_bind;
    bool attach_to_radius;
public:
    bool read(Vector2& out)
    {
        Vector3 pos;
        if (!calculate_entity_position (py_input_bind.group, pos))
        {
            return false;
        }
        Vector2 target_pos (pos.x, pos.z);
        Vector2 delta;
        float angle;
        float distance = get_radar_distance (target_pos, delta, angle);

        if (attach_to_radius && distance > context.radius_big)
        {
            out.x = context.base_position.x + cos (angle) * context.radius_big * context.scale;
            out.y = - (context.base_position.y + sin (angle) * context.radius_big * context.scale);
        }
        else
        {
            out.x = context.base_position.x + (delta.x) * context.scale;
            out.y = - (context.base_position.y + (delta.y) * context.scale);
        }

        return true;
    }

    RadarPositionInput (const PyInputBind& input_bind): py_input_bind (input_bind), entityID_(input_bind.entity_id), attach_to_radius (true) 
    {
        if (py_input_bind.config_object.IsNull ()) return;
        context.from_gfx_value (py_input_bind.config_object);
        get_bool_value (py_input_bind.config_object, "attach_to_radius", attach_to_radius);
    }
};

///////////////////////////////////////////////////

class RadarVisibilityInput: public Router::IInput <bool>
{
    RadarMinimapContext context;
    EntityID entityID_;
    PyInputBind py_input_bind;
    bool initialized;
public:
    bool read (bool& out)
    {
        Vector3 pos;
        if (!calculate_entity_position (py_input_bind.group, pos))
        {
            return false;
        }
        Vector2 target_pos (pos.x, pos.z);
        float angle;
        Vector2 delta;
        float distance = get_radar_distance (target_pos, delta, angle);
        bool visible = distance < context.radius_big;

        bool dirty = false;
        if (out != visible)
        {
            dirty = true;
        }

        out = visible;
        if (!initialized)
        {
            initialized = true;
            return true;
        }

        return dirty;
    }
    virtual void on_activate () override
    {
        initialized = false;
    }

    RadarVisibilityInput (const PyInputBind& input_bind): py_input_bind (input_bind), entityID_(input_bind.entity_id), initialized (false) 
    {
        if (py_input_bind.config_object.IsNull ()) return;
        context.from_gfx_value (py_input_bind.config_object);
    }
};

///////////////////////////////////////////////////

class DistanceInput: public Router::IInput<float>
{
    EntityID entityID_;
    float scale;
public:
    bool read(float& out)
    {
        const EntityID spectatorID = EntitiesHud::getSpectatorEntityID();
        Entity * player = spectatorID > 0 ? EntityManager::instance().getEntity(spectatorID, true) : ScriptPlayer::instance().entity();

        if (player == NULL)
        {
            return false;
        }

        Entity * entity = EntityManager::instance().getEntity (entityID_, true);
        if (entity == NULL)
        {
//            TRACE_MSG("[DistanceInput]: wrong entity id: %d\n", entityID_);
            return false;
        }

        Vector3 pos, dim;
        float dist = (entity->position() - player->position()).length() * scale;
        out = dist;

        return true;
    }

    DistanceInput (const PyInputBind& py_input_bind): entityID_(py_input_bind.entity_id), scale (1.0f) 
    {
        if (!py_input_bind.config_object.IsNull ())
        {
            get_float_value (py_input_bind.config_object, "scale", scale);
        }
    }
};

///////////////////////////////////////////////////

class DirectionToCameraInput: public Router::IInput<float>
{
	static BW::vector<int> __debugLayer;

    EntityID entityID_;
	bool firstRun = true;
public:
	virtual ~DirectionToCameraInput() override
	{
		std::remove_if(std::begin(__debugLayer), std::end(__debugLayer), [this](int i)
		{
			return (int)this == i;
		});
	}

    bool read(float& out)
    {

        Entity * entity = EntityManager::instance().getEntity (entityID_, true);
        if (entity == NULL)
        {
//            TRACE_MSG("[DirectionToCameraInput]: wrong entity id: %d\n", entityID_);
            return false;
        }

		if (firstRun)
		{
			__debugLayer.emplace_back((int)this);
		}

		int debugIndex = 0;
		auto it = std::find(std::begin(__debugLayer), std::end(__debugLayer), (int)this);
		if (it != std::end(__debugLayer))
		{
			debugIndex = std::distance(std::begin(__debugLayer), it);
		}

        float angle = calculate_offscreen_direction (entity);
        if (angle < 0.0f) angle += MATH_2PI;
        if (angle > MATH_2PI) angle -= MATH_2PI;
        out = angle;
#ifdef ROUTES_DEBUG
		Vector2 center(Moo::rc().halfScreenWidth(), Moo::rc().halfScreenHeight());
		BW::stringstream ss;
		ss << "@#$ Dirrection: " << angle * 180 / MATH_PI;
		DebugDrawApp::instance.AddText("Routing", ss.str(), (int)center.x, (int)center.y - (debugIndex * 30), 0xFF0000FF);
		//DebugDrawApp::instance.Add2DRect("Routing", Vector2(center.x, center.y), Vector2(out.z, out.w), 0xFF0000FF);
#endif
        return true;
    }
    DirectionToCameraInput (const PyInputBind& py_input_bind): entityID_(py_input_bind.entity_id) {}
};

BW::vector<int> DirectionToCameraInput::__debugLayer;


class PositionToCameraInput: public Router::IInput<Vector2>
{
    Vector2     align_;
    EntityID entityID_;
	float	radiusMax_;
public:
    bool read(Vector2& out)
    {
        Entity * entity = EntityManager::instance().getEntity (entityID_, true);
        if (entity == NULL)
        {
//            TRACE_MSG("[PositionToCameraInput]: wrong entity id: %d\n", entityID_);
            return false;
        }

		out = calculate_offscreen_position (entity, align_);
		Vector2 center(Moo::rc().halfScreenWidth(), Moo::rc().halfScreenHeight());
		auto delta = out - center;
		auto deltaClamped = clampVectorMagnitude (delta, radiusMax_);
		out = center + deltaClamped;

        return true;
    }
	PositionToCameraInput (const PyInputBind& py_input_bind): entityID_(py_input_bind.entity_id), align_ (0.0f, 0.0f), radiusMax_(0.0f)
    {
        get_float_value (py_input_bind.config_object, "shift_x", align_.x);
        get_float_value (py_input_bind.config_object, "shift_y", align_.y);
		
		if (!get_float_value(py_input_bind.config_object, "radius_max", radiusMax_))
		{
			radiusMax_ = 10000.0f;
		}
    }
};


///////////////////////////////////////////////////

class VisibleInput: public Router::IInput <bool>
{
    bool initialized;
    EntityID entityID_;
public:
    bool read(bool& out)
    {
        Entity * entity = EntityManager::instance().getEntity (entityID_, true);
        if (entity == NULL)
        {
//            TRACE_MSG("[VisibleInput]: wrong entity id: %d\n", entityID_);
            return false;
        }

        Vector3 angles = calculate_entity_angles (entity);

        float aspect = Moo::rc().camera ().aspectRatio();
        float y_fov = Moo::rc().camera ().fov () / 2;
        float x_fov = y_fov * aspect;

        bool visible = (angles.z > 0.0f) && (abs (angles.x) < x_fov) && (abs (angles.y) < y_fov);

        bool dirty = false;
        if (out != visible)
        {
            dirty = true;
        }

        out = visible;
        if (!initialized)
        {
            initialized = true;
            return true;
        }

        return dirty;
    }
    virtual void on_activate () override
    {
        initialized = false;
    }
    VisibleInput (const PyInputBind& py_input_bind): entityID_(py_input_bind.entity_id), initialized (false) {}
};

///////////////////////////////////////////////////

class TopBottomInput: public Router::IInput <bool>
{
    bool initialized;
    EntityID entityID_;
    EntityID player_id;
public:
    bool read(bool& out)
    {
        Entity* entity = EntityManager::instance().getEntity (entityID_, true);
        if (entity == NULL) return false;
        Entity* player = NULL;
        if (player_id != -1) 
        {
            player = EntityManager::instance().getEntity (player_id, true);
        }
        bool on_top = false;

        if (player != NULL)
        {
            on_top = player->position().y < entity->position().y;
        }
        else
        {
            Vector3 angles = calculate_entity_angles (entity);
            on_top = angles.y > 0;
        }
        
        bool dirty = out != on_top;
        out = on_top;
        if (!initialized)
        {
            initialized = true;
            return true;
        }

        return dirty;
    }
    virtual void on_activate () override
    {
        initialized = false;
    }
    TopBottomInput (const PyInputBind& py_input_bind): entityID_(py_input_bind.entity_id), initialized (false), player_id (-1) 
    {
        get_int_value (py_input_bind.config_object, "player_id", player_id);
    }
};

///////////////////////////////////////////////////

class FrontBackInput: public Router::IInput <bool>
{
    bool initialized;
    EntityID entityID_;
public:
    bool read(bool& out)
    {
        Entity* entity = EntityManager::instance().getEntity (entityID_, true);
        if (entity == NULL)
        {
//            TRACE_MSG("[VisibleInput]: wrong entity id: %d\n", entityID_);
            return false;
        }
        Vector3 angles = calculate_entity_angles (entity);
        bool at_front = angles.z > 0.0f;
        
        bool dirty = out != at_front;
        out = at_front;
        if (!initialized)
        {
            initialized = true;
            return true;
        }

        return dirty;
    }
    virtual void on_activate () override
    {
        initialized = false;
    }
    FrontBackInput (const PyInputBind& py_input_bind): entityID_(py_input_bind.entity_id), initialized (false) {}
};

///////////////////////////////////////////////////

class ApproachingInput: public Router::IInput <bool>
{
    bool initialized;
    float stored_distance;
    EntityID entityID_;
public:
    bool read(bool& out)
    {
        Entity* entity = EntityManager::instance().getEntity (entityID_, true);
        if (entity == NULL)
        {
            return false;
        }

        const Vector3& entity_pos = entity->position ();
	    Matrix invView = Moo::rc().invView();
	    Vector3 camera_pos (invView._41, invView._42, invView._43);

        Vector3 delta = entity_pos - camera_pos;
        float new_distance = delta.lengthSquared ();

        bool dirty = false;
        if (!initialized)
        {
            initialized = true;
            out = false;
            dirty = true;
        }
        else
        {
            bool approaching = new_distance < stored_distance;
            dirty = out != approaching;
            out = approaching;
        }
        stored_distance = new_distance;

        return dirty;
    }
    virtual void on_activate () override
    {
        initialized = false;
    }
    ApproachingInput (const PyInputBind& py_input_bind): entityID_(py_input_bind.entity_id), initialized (false), stored_distance (0.0f) {}
};


///////////////////////////////////////////////////

class DistanceToTerrain2DInput: public Router::IInput <Vector2>
{
    PyInputBind py_input_bind;
    Vector3 groud_point;
public:
    bool read (Vector2& out)
    {
        Entity* entity = EntityManager::instance().getEntity (py_input_bind.entity_id, true);
        if (entity == NULL)
        {
//            TRACE_MSG("[PositionInput]: wrong entity id: %d\n", entityID_);
            return false;
        }

        Vector2 entity_pos_2d;
        Vector2 groud_point_2d;

        out.x = 1.0f;
        out.y = 1.0f;
        const auto entity_pos = entity->position();

#ifdef ROUTES_DEBUG
        //DebugDrawApp::instance.AddLine("Routing", entity_pos - Vector3(20, 20, 20), entity_pos + Vector3(20, 20, 20), 0xFF00FFFF, false);
        //DebugDrawApp::instance.AddLine("Routing", entity_pos - Vector3(-20, 20, 20), entity_pos + Vector3(-20, 20, 20), 0xFF00FFFF, false);
        //DebugDrawApp::instance.AddLine("Routing", entity_pos - Vector3(-20, -20, 20), entity_pos + Vector3(-20, -20, 20), 0xFF00FFFF, false);

        //DebugDrawApp::instance.AddLine("Routing", groud_point - Vector3(20, 20, 20), groud_point + Vector3(20, 20, 20), 0xFFFF00FF, false);
        //DebugDrawApp::instance.AddLine("Routing", groud_point - Vector3(-20, 20, 20), groud_point + Vector3(-20, 20, 20), 0xFFFF00FF, false);
        //DebugDrawApp::instance.AddLine("Routing", groud_point - Vector3(-20, -20, 20), groud_point + Vector3(-20, -20, 20), 0xFFFF00FF, false);
#endif

        if (world_to_screen (entity_pos, entity_pos_2d) && world_to_screen (groud_point, groud_point_2d))
        {
#ifdef ROUTES_DEBUG
            //DebugDrawApp::instance.Add2DRect("Routing", entity_pos_2d - Vector2(10, 10), entity_pos_2d + Vector2(10, 10), 0xFF00FFFF);
            //DebugDrawApp::instance.Add2DRect("Routing", groud_point_2d - Vector2(10, 10), groud_point_2d + Vector2(10, 10), 0xFFFF00FF);
#endif
            out = groud_point_2d - entity_pos_2d;
        }

        return true;
    }

    DistanceToTerrain2DInput (const PyInputBind& input_bind): py_input_bind (input_bind) 
    {
        if (!py_input_bind.config_object.IsNull ())
        {
            get_float_value (py_input_bind.config_object, "x", groud_point.x);
            get_float_value (py_input_bind.config_object, "y", groud_point.y);
            get_float_value (py_input_bind.config_object, "z", groud_point.z);
        }
    }
};


///////////////////////////////////////////////////


class PythonValueInput: public Router::IInput<BW::string>
{
    std::string name_;
    std::vector <std::string> path_;
    EntityID entityID_;
public:
    bool read (BW::string& out)
    { 
        Entity * entity = EntityManager::instance().getEntity (entityID_, true);
        if (entity == NULL)
        {
            TRACE_MSG("[PythonValueInput]: wrong entity id: %d\n", entityID_);
            return false;
        }

        PyObject* obj = findObject (static_cast<PyObject*> (entity->pPyEntity().get()), name_, path_);
        BW::string new_value = jsonEncodeString (obj).c_str ();
        bool dirty = (out != new_value);
        out = new_value;
        return dirty; 
    }
    PythonValueInput(const PyInputBind& py_input_bind): entityID_ (py_input_bind.entity_id), name_ (py_input_bind.var_name)
    {
        split(name_, '.', path_);
    }
};


class PythonWStrInput: public Router::IInput<BW::wstring>
{
    std::string name_;
    std::vector<std::string> path_;
    EntityID entityID_;
public:
    bool read (BW::wstring& out)
    { 
        Entity * entity = EntityManager::instance().getEntity (entityID_, true);
        if (entity == NULL)
        {
            TRACE_MSG("[PythonWStrInput]: wrong entity id: %d\n", entityID_);
            return false;
        }

        PyObject* obj = findObject (static_cast<PyObject*> (entity->pPyEntity().get()), name_, path_);
        BW::wstring new_value;
        if (PyUnicode_CheckExact(obj))
        {
            PyObject* utf8 = PyUnicode_AsUTF8String(obj);
            MF_ASSERT (utf8 != NULL);
            new_value = bw_utf8tow (PyString_AsString (utf8));
            Py_DECREF (utf8);
        }
        else if (PyString_CheckExact(obj))
        {
            new_value = bw_utf8tow (PyString_AsString (obj));
        }

        bool dirty = (out != new_value);
        out = new_value;
        return dirty; 
    }
    PythonWStrInput(const PyInputBind& py_input_bind): entityID_ (py_input_bind.entity_id), name_ (py_input_bind.var_name) 
    {
        split(name_, '.', path_);
    }

};


#ifdef ROUTES_DEBUG
#pragma optimize("", on)
#endif

}

BW_END_NAMESPACE

void registerHammerInputs ()
{
    using namespace BW;
    using namespace BW::Hammer;
    Router::Router::registerInput <PositionInput, PyInputBind> ("ReadPosition");
    Router::Router::registerInput <PartPositionInput, PyInputBind> ("PartPositionInput");

    Router::Router::registerInput <DistanceInput, PyInputBind> ("DistanceInput");
    Router::Router::registerInput <VisibleInput, PyInputBind> ("VisibleInput");

    Router::Router::registerInput <DirectionToCameraInput, PyInputBind> ("DirectionToCameraInput");
    Router::Router::registerInput <PositionToCameraInput, PyInputBind> ("PositionToCameraInput");

    Router::Router::registerInput <MiniMapPositionInput, PyInputBind> ("MiniMapPositionInput");
    Router::Router::registerInput <MiniMapRotationInput, PyInputBind> ("MiniMapRotationInput");
    Router::Router::registerInput <MiniMapCameraRotationInput, PyInputBind> ("MiniMapCameraRotationInput");

    Router::Router::registerInput <RadarPositionInput, PyInputBind> ("RadarPositionInput");
    Router::Router::registerInput <RadarVisibilityInput, PyInputBind> ("RadarVisibilityInput");
    
    Router::Router::registerInput <DistanceToTerrain2DInput, PyInputBind> ("DistanceToTerrain2DInput");

    Router::Router::registerInput <EntityPartsBBoxInput, PyInputBind> ("EntityPartsBBoxInput");

    Router::Router::registerInput <TopBottomInput, PyInputBind> ("TopBottomInput");
    Router::Router::registerInput <FrontBackInput, PyInputBind> ("FrontBackInput");
    Router::Router::registerInput <ApproachingInput, PyInputBind> ("ApproachingInput");

    Router::Router::registerInput <PythonValueInput, PyInputBind> ("PythonValueInput");
    Router::Router::registerInput <PythonWStrInput, PyInputBind> ("PythonWStrInput");

	Router::Router::registerInput <OrientationMarkerPositionInput, PyInputBind>("OrientationMarkerPositionInput");
    Router::Router::registerInput <OrientationMarkerRotationInput, PyInputBind>("OrientationMarkerRotationInput");
	Router::Router::registerInput <OrientationMarkerVisibilityInput, PyInputBind>("OrientationMarkerVisibilityInput");
}
