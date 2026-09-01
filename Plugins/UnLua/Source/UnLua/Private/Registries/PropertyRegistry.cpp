#include "PropertyRegistry.h"
#include "Binding.h"
#include "ClassRegistry.h"
#include "EnumRegistry.h"
#include "LowLevel.h"
#include "LuaEnv.h"
#include "ReflectionUtils/PropertyDesc.h"

namespace UnLua
{
    FPropertyRegistry::FPropertyRegistry(FLuaEnv* Env)
        : Env(Env)
    {
        PropertyCollector = FindFirstObject<UScriptStruct>(TEXT("PropertyCollector"));
        check(PropertyCollector);
    }

    void FPropertyRegistry::NotifyUObjectDeleted(UObject* Object)
    {
        FieldProperties.Remove(static_cast<UField*>(Object));
    }

    TSharedPtr<ITypeInterface> FPropertyRegistry::CreateTypeInterface(lua_State* L, int32 Index)
    {
        Index = LowLevel::AbsIndex(L, Index);

        TSharedPtr<ITypeInterface> TypeInterface;
        int32 Type = lua_type(L, Index);
        switch (Type)
        {
        case LUA_TBOOLEAN:
            TypeInterface = GetBoolProperty();
            break;
        case LUA_TNUMBER:
            TypeInterface = lua_isinteger(L, Index) > 0 ? GetIntProperty() : GetFloatProperty();
            break;
        case LUA_TSTRING:
            TypeInterface = GetStringProperty();
            break;
        case LUA_TTABLE:
            {
                lua_pushstring(L, "__name");
                Type = lua_rawget(L, Index);
                if (Type == LUA_TSTRING)
                {
                    const char* Name = lua_tostring(L, -1);
                    auto ClassDesc = Env->GetClassRegistry()->Find(Name);
                    if (ClassDesc)
                    {
                        TypeInterface = GetFieldProperty(ClassDesc->AsStruct());
                    }
                    else
                    {
                        auto EnumDesc = Env->GetEnumRegistry()->Find(Name);
                        if (EnumDesc)
                            TypeInterface = GetFieldProperty(EnumDesc->GetEnum());
                        else
                            TypeInterface = FindTypeInterface(lua_tostring(L, -1));
                    }
                }
                lua_pop(L, 1);
            }
            break;
        case LUA_TUSERDATA:
            {
                // mt/nil
                lua_getmetatable(L, Index);
                if (lua_istable(L, -1))
                {
                    // mt,mt.__name/nil
                    lua_getfield(L, -1, "__name");
                    if (lua_isstring(L, -1))
                    {
                        const char* Name = lua_tostring(L, -1);
                        FClassDesc* ClassDesc = Env->GetClassRegistry()->Find(Name);
                        if (ClassDesc)
                            TypeInterface = GetFieldProperty(ClassDesc->AsStruct());
                    }
                    // mt
                    lua_pop(L, 1);
                }
                lua_pop(L, 1);
            }
            break;
        default:
            break;
        }

        return TypeInterface;
    }

    TSharedPtr<ITypeInterface> FPropertyRegistry::GetBoolProperty()
    {
        if (!BoolProperty)
        {
            const auto Property = new FBoolProperty(PropertyCollector, NAME_None);
            Property->SetBoolSize(sizeof(bool), true);
            Property->SetPropertyFlags(CPF_HasGetValueTypeHash);
            BoolProperty = TSharedPtr<ITypeInterface>(FPropertyDesc::Create(Property));
        }
        return BoolProperty;
    }

    TSharedPtr<ITypeInterface> FPropertyRegistry::GetIntProperty()
    {
        if (!IntProperty)
        {
            const auto Property = new FIntProperty(PropertyCollector, NAME_None);
            Property->SetPropertyFlags(CPF_HasGetValueTypeHash);
            IntProperty = TSharedPtr<ITypeInterface>(FPropertyDesc::Create(Property));
        }
        return IntProperty;
    }

    TSharedPtr<ITypeInterface> FPropertyRegistry::GetFloatProperty()
    {
        if (!FloatProperty)
        {
            const auto Property = new FFloatProperty(PropertyCollector, NAME_None);
            Property->SetPropertyFlags(CPF_HasGetValueTypeHash);
            FloatProperty = TSharedPtr<ITypeInterface>(FPropertyDesc::Create(Property));
        }
        return FloatProperty;
    }

    TSharedPtr<ITypeInterface> FPropertyRegistry::GetStringProperty()
    {
        if (!StringProperty)
        {
            const auto Property = new FStrProperty(PropertyCollector, NAME_None);
            Property->SetPropertyFlags(CPF_HasGetValueTypeHash);
            StringProperty = TSharedPtr<ITypeInterface>(FPropertyDesc::Create(Property));
        }
        return StringProperty;
    }

    TSharedPtr<ITypeInterface> FPropertyRegistry::GetNameProperty()
    {
        if (!NameProperty)
        {
            const auto Property = new FNameProperty(PropertyCollector, NAME_None);
            Property->SetPropertyFlags(CPF_HasGetValueTypeHash);
            NameProperty = TSharedPtr<ITypeInterface>(FPropertyDesc::Create(Property));
        }
        return NameProperty;
    }

    TSharedPtr<ITypeInterface> FPropertyRegistry::GetTextProperty()
    {
        if (!TextProperty)
        {
            const auto Property = new FTextProperty(PropertyCollector, NAME_None);
            Property->SetPropertyFlags(CPF_HasGetValueTypeHash);
            TextProperty = TSharedPtr<ITypeInterface>(FPropertyDesc::Create(Property));
        }
        return TextProperty;
    }

    TSharedPtr<ITypeInterface> FPropertyRegistry::GetFieldProperty(UField* Field)
    {
        if (const auto Exists = FieldProperties.Find(Field))
            return *Exists;

        FProperty* Property;
        if (const auto Class = Cast<UClass>(Field))
        {
            const auto ObjectProperty = new FObjectProperty(PropertyCollector, NAME_None);
            ObjectProperty->PropertyClass = Class;
            ObjectProperty->SetPropertyFlags(CPF_HasGetValueTypeHash);
            Property = ObjectProperty;
        }
        else if (const auto ScriptStruct = Cast<UScriptStruct>(Field))
        {
            const auto StructProperty = new FStructProperty(PropertyCollector, NAME_None);
            StructProperty->Struct = ScriptStruct;
            StructProperty->SetElementSize(ScriptStruct->PropertiesSize);
            StructProperty->SetPropertyFlags(
                ScriptStruct->GetCppStructOps()
                    ? ScriptStruct->GetCppStructOps()->GetComputedPropertyFlags() | CPF_HasGetValueTypeHash
                    : CPF_HasGetValueTypeHash);
            Property = StructProperty;
        }
        else if (const auto Enum = Cast<UEnum>(Field))
        {
            const auto EnumProperty = new FEnumProperty(PropertyCollector, NAME_None);
            EnumProperty->SetEnum(Enum);
            const auto UnderlyingProperty = new FByteProperty(EnumProperty, TEXT("UnderlyingType"));
            UnderlyingProperty->SetPropertyFlags(CPF_HasGetValueTypeHash);
            Property = EnumProperty;
            Property->AddCppProperty(UnderlyingProperty);
            Property->SetElementSize(UnderlyingProperty->GetElementSize());
            Property->SetPropertyFlags(CPF_HasGetValueTypeHash | CPF_IsPlainOldData | CPF_NoDestructor | CPF_ZeroConstructor);
        }
        else
        {
            Property = nullptr;
        }

        const auto Ret = TSharedPtr<ITypeInterface>(FPropertyDesc::Create(Property));
        FieldProperties.Add(Field, Ret);
        return Ret;
    }
}
